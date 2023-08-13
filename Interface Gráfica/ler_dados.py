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

# ==== MAIN ====

QUANT = 10

temp, umid, co2 = [], [], []
temp_med, umid_med, co2_med = 0.0, 0.0, 0.0
i = 0

with serial.Serial('COM4', 115200, serial.FIVEBITS, serial.PARITY_NONE, serial.STOPBITS_ONE, timeout = 1) as ser:
    while(1):
        line = ser.readline()
        if line != b'\r':
            vet = []
            for x in line:
                xis = int(x)
                if xis >= 48 and int(x) <= 57:
                    vet.append(xis-48)
                else:
                    vet.append(x)
            if len(vet) > 0:
                vet_inis = achar_inis(*vet)
            
                temp.append(ler_dado(0, *vet))
                umid.append(ler_dado(vet_inis[0], *vet))
                co2.append(ler_dado(vet_inis[1], *vet))

                temp_med = sum(temp)/len(temp)
                umid_med = sum(umid)/len(umid)
                co2_med = sum(co2)/len(co2)

                print(f"=== Medida número {i+1} ===")
                print(f"Média de temperatura: {temp_med:6.1f} ºC")
                print(f"Média de umidade: {umid_med:10.1f} %")
                print(f"Média de CO2: {co2_med:15.1f} PPM")

                i += 1
                
                if i >= QUANT:
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
                    break
                
