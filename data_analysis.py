import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker

#insert the csv file generated from the scan_get_data file
data = pd.read_csv("2024_06_07_data.csv")
print(data)

D = data.to_numpy()
time = D[:,0]
temperature = D[:,1]
humidity = D[:,2]

counter = 1
temp_avg = 0
hum_avg = 0
#if there are too much data points, truncates the time specificity of seconds
#only displaying hour and minutes
if (len(time) > 10):
    for i in range(len(time)):
        time[i] = time[i][:5]

    new_time = []
    new_temp = []
    new_hum = []
    need_to_add = False

    for i in range(len(time) - 1):
        temp_avg += temperature[i]
        hum_avg += humidity[i]
        if time[i] == time[i+1] and i != (len(time) - 2):
            counter += 1
        else:
            #if its the last data point, compare to previous point
            if (i  == len(time) - 2):
                if time[i+1] == time[i]:
                    counter += 1
                    temp_avg += temperature[i+1]
                    hum_avg += humidity[i+1]
                else:
                    need_to_add = True
            new_time.append(time[i])
            new_temp.append(round(temp_avg/counter,1))
            new_hum.append(round(hum_avg/counter,1))
            counter = 1
            temp_avg = 0
            hum_avg = 0

    #for edge case of singular data point on its own
    if need_to_add:
        new_time.append(time[len(time) - 1])
        new_temp.append(temperature[len(temperature) - 1])
        new_hum.append(humidity[len(humidity) - 1])

    time = new_time
    temperature = new_temp
    humidity = new_hum

figure, axis = plt.subplots(1,2)
axis[0].plot(time,temperature)
axis[0].set_title("Temperature Over Time")
axis[0].set_xlabel("Time")
axis[0].set_ylabel("Temperature (°C)")
axis[0].xaxis.set_major_locator(ticker.MaxNLocator(8))

axis[1].plot(time,humidity)
axis[1].set_title("Humidity Over Time")
axis[1].set_xlabel("Time")
axis[1].set_ylabel("Humidity")
axis[1].xaxis.set_major_locator(ticker.MaxNLocator(8))

plt.show()
