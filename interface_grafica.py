import tkinter as tk
import matplotlib.pyplot as plt
import serial
from datetime import datetime

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
    if lin[ini+1] == 44:    # ascii para vÃ­rgula
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

# ==== FunÃ§Ãµes TKINTER =====

def leitura():
    ser = serial.Serial('COM4', 115200, serial.FIVEBITS, serial.PARITY_NONE,
                        serial.STOPBITS_ONE, timeout = 2.5)
    line = ser.readline()
    global i, temp_med, umid_med, co2_med, temp, umid, co2, window, parar, button
    if button[2]['state'] == 'normal':
        for b in range(2,5):
            button[b]['state'] = 'disabled'
    if parar == 1:
        parar = 0
        for b in range(2,5):
            button[b]['state'] = 'normal'
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

            mt = len(temp)-1
            lin_dados = f"{temp[mt]} {umid[mt]} {co2[mt]}\n"
            txt = open(dt_string,"a+")
            txt.write(lin_dados)
            txt.close()
            
            temp_med = sum(temp)/len(temp)
            umid_med = sum(umid)/len(umid)
            co2_med = sum(co2)/len(co2)

            tempo_h = (i*2.5)/3600
            tempo_m = (tempo_h%1)*60
            tempo_s = (tempo_m%1)*60
            tempo_h = int(tempo_h)
            tempo_m = int(tempo_m)
            tempo_s = int(tempo_s)

            global terminal
            terminal['text'] = f"NÃºmero de medidas: {i+1}\n"
            terminal['text'] += f"Tempo: {tempo_h}:{tempo_m}:{tempo_s}\n\n"

            terminal['text'] += f"Temperatura: {temp[i]:6.1f} ÂºC\n"
            terminal['text'] += f"Umidade: {umid[i]:12.1f} %\n"
            terminal['text'] += f"CO2: {co2[i]:18.1f} PPM\n\n"
            
            terminal['text'] += f"MÃ©dia de temperatura: {temp_med:6.1f} ÂºC\n"
            terminal['text'] += f"MÃ©dia de umidade: {umid_med:12.1f} %\n"
            terminal['text'] += f"MÃ©dia de CO2: {co2_med:18.1f} PPM"

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
    plt.ylabel('temperatura [ÂºC]')
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
    plt.title('NÃ­vel de CO2')
    
    plt.show()

def manda_x():
    ser = serial.Serial('COM4', 115200, serial.FIVEBITS, serial.PARITY_NONE,
                        serial.STOPBITS_ONE, timeout = 2.5)
    global i
    if i>0:
        ser.write(b'x\n\r')
        line = ser.readline()
        print(line)
    line = entry[0].get() + ' ' + entry[1].get() + "\n\r"
    ser.write(b'{line}')
    line = ser.readline()
    print(line)
    line = entry[2].get() + ' ' + entry[3].get() + "\n\r"
    ser.write(b'{line}')
    line = ser.readline()
    print(line)
    line = entry[4].get() + "\n\r"
    ser.write(b'{line}')
    line = ser.readline()
    print(line)
    while(1):
        if b'#' or b'>' or b'<' in line:
            line = ser.readline()
            print(line)
            break
        line = ser.readline()
        print(line)
    ser.close()

def manda_xx():
    #for i in range(len(entry)):
    #    print(entry[i].get())
    line = entry[0].get() + ' ' + entry[1].get() + "\n\r"
    print(line)

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

now = datetime.now()
dt_string = now.strftime("COG_%d-%m-%Y_%H-%M-%S")+".txt"

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
