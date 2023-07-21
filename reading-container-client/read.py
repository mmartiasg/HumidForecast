import requests
import time
from datetime import datetime
import os

#in seconds
MINUTE = 60
TIME_LAPSE = 10*MINUTE

# write headers
if not os.path.exists("measurements.csv"):
    with open("measurements.csv", "w") as f:
        f.write("time, room, humidity, temp_celcius, temp_fahrenheit, temp_index_celcius, temp_index_fahrenheit"+"\n")

while True:
    response = requests.get("http://192.168.1.138")
    if response.status_code == 200:
        measure = response.json()
        with open("measurements.csv", "+a") as f:
            row = ", ".join([datetime.now().strftime("%Y:%m:%d %H:%M:%S"),
                            "living",
                            str(measure['Humidity']),
                            str(measure['C']),
                            str(measure['F']),
                            str(measure['C_H_index']),
                            str(measure['F_H_index'])])

            # f.write(datetime.now().strftime("%Y:%m:%d %H:%M:%S"))
            # f.write(", ")
            # f.write("living")
            # f.write(", ")
            # f.write(str(measure['Humidity']))
            # f.write(", ")
            # f.write(str(measure['C']))
            # f.write(", ")
            # f.write(str(measure['F']))
            # f.write(", ")
            # f.write(str(measure['C_H_index']))
            # f.write(", ")
            # f.write(str(measure['F_H_index']))
            # f.write("\n")

            f.write(row+"\n")
        print(response.text)
    else:
        print("Error getting meassurements")
    
    time.sleep(TIME_LAPSE)
