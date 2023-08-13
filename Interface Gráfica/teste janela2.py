import tkinter as tk
import matplotlib.pyplot as plt
import serial

def achar_inis(*lin):
    vet = [0,0]
    i = 0
    passos = 0
    for x in lin:
        if x == 35 or x == 60 or x == 62:   # ascii para #, < e >
            vet[i] = passos + 2
            i += 1
            if i == 2:
                break
        passos += 1
    return vet

def ler_dado(ini, *lin):
    if lin[ini+1] == 44:    # ascii para vírgula
        dado = int(lin[ini])
        ini += - 1
    elif lin[ini+2] == 44:
        dado = int(lin[ini])*10.0 + int(lin[ini+1])
    elif lin[ini+3] == 44:
        dado = int(lin[ini])*100.0 + int(lin[ini+1])*10 + int(lin[ini+2])
        ini += 1
    else:
        dado = int(lin[ini])*1000.0 + int(lin[ini+1])*100 + int(lin[ini+2])*10 + int(lin[ini+3])
        ini += 2
    aux = float((int(lin[ini+3])*100 + int(lin[ini+4])*10 + int(lin[ini+5]))/1000)
    dado += aux
    return dado

# ==== Funções TKINTER =====

def leitura():
    ser = serial.Serial('COM4', 115200, serial.FIVEBITS, serial.PARITY_NONE,
                        serial.STOPBITS_ONE, timeout = 2.5)
    line = ser.readline()
    global i, temp_med, umid_med, co2_med, temp, umid, co2, window, parar
    if parar == 1:
        parar = 0
        return 0
    if line != b'\r':
        vet = []
        for x in line:
            xis = int(x)
            if xis >= 48 and int(x) <= 57:
                vet.append(xis-48)
            elif x != 13: #remover carriage return
                vet.append(x)
        if len(vet) > 0:
            vet_inis = achar_inis(*vet)
        
            temp.append(ler_dado(0, *vet))
            umid.append(ler_dado(vet_inis[0], *vet))
            co2.append(ler_dado(vet_inis[1], *vet))

            temp_med = sum(temp)/len(temp)
            umid_med = sum(umid)/len(umid)
            co2_med = sum(co2)/len(co2)

            global terminal
            terminal['text'] = f"Medida número {i+1}\n\nMédia de temperatura: {temp_med:6.1f} ºC\nMédia de umidade: {umid_med:12.1f} %\nMédia de CO2: {co2_med:18.1f} PPM"

            i += 1

            window.after(1000, leitura)
            
            ser.close()

def graf():
    tempo = [0.0]
    for j in range(1,len(temp)):
        tempo.append(j*2.5)
    
    plt.figure()
    plt.plot(tempo, temp, 'bo', ms = 2)
    plt.plot([tempo[0],tempo[len(tempo)-1]], [temp_med, temp_med], 'b--')
    plt.xlabel('tempo [s]')
    plt.ylabel('temperatura [ºC]')
    plt.title('Temperatura')

    plt.figure()
    plt.plot(tempo, umid, 'go', ms = 2)
    plt.plot([tempo[0],tempo[len(tempo)-1]], [umid_med, umid_med], 'g--')
    plt.xlabel('tempo [s]')
    plt.ylabel('umidade [%]')
    plt.title('Umidade Relativa')

    plt.figure()
    plt.plot(tempo, co2, 'ro', ms = 2)
    plt.plot([tempo[0],tempo[len(tempo)-1]], [co2_med, co2_med], 'r--')
    plt.xlabel('tempo [s]')
    plt.ylabel('CO2 [PPM]')
    plt.title('Nível de CO2')
    
    plt.show()

def manda_x():
    ser = serial.Serial('COM4', 115200, serial.FIVEBITS, serial.PARITY_NONE,
                        serial.STOPBITS_ONE, timeout = 2.5)
    if i>0:
        ser.write(b'x')
        ser.write(b'\n')
        line = ser.readline()
        print(line)
    line = b''
    while(1):
        if b'PPM' not in line:
            ser.write(b'1')
            ser.write(b'2')
            ser.write(b' ')
            ser.write(b'3')
            ser.write(b'4')
            ser.write(b'\n')
        else:
            ser.write(b'5')
            ser.write(b'6')
            ser.write(b'7')
            ser.write(b'\n')
            line = ser.readline()
            print(line)
            break
        line = ser.readline()
        print(line)
    ser.close()

def ler_term():
    ser = serial.Serial('COM4', 115200, serial.FIVEBITS, serial.PARITY_NONE,
                        serial.STOPBITS_ONE, timeout = 2.5)

    line = ser.readline()
    print(line)
    
    ser.close()

def stop():
    global parar
    parar = 1

# ==== MAIN ====

temp, umid, co2 = [], [], []
temp_med, umid_med, co2_med = 0.0, 0.0, 0.0
temp_def, umid_def, co2_def = [0,0], [0,0], 0
i = 0
parar = 0

window = tk.Tk()

window.title("LEITOR COGUMELO")
window.iconbitmap('favicon.ico')

labels = ['Temp min','Temp max','Umid min','Umid max','CO2 max']
entry = [0,0,0,0,0]
frame1 = tk.Frame(master=window,relief='ridge',borderwidth=5,padx=10,pady=10)
frame1.grid(row=0,column=0)
for j in range(0,5):
    label = tk.Label(master=frame1, text=labels[j])
    label.pack()
    entry[j] = tk.Entry(master=frame1)
    entry[j].pack()

but_lab = ['LER','STOP','GRAF','NOVO','TERM']
but_fun = [leitura,stop,graf,manda_x,ler_term]
button = [0,0,0,0,0]
frame2 = tk.Frame(master=window,borderwidth=5,padx=10,pady=10)
frame2.grid(row=0,column=1)
for j in range(0,5):
    button[j] = tk.Button(master=frame2,text=but_lab[j],width=5,height=2,
                          command=but_fun[j])
    button[j].pack()

frame3 = tk.Frame(master=window,relief='groove',borderwidth=5,padx=10,pady=10)
frame3.grid(row=0,column=2)
terminal = tk.Label(master=frame3,text='Aguardando leitura',width=30,height=10,
                    padx=10,pady=10,relief='groove',font="Arial 12",
                    justify='left')
terminal.pack()

window.mainloop() 
