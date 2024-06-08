import csv
import asyncio
from bleak import BleakScanner, BleakClient
from PyObjCTools import KeyValueCoding
from datetime import datetime

filename = str(datetime.now().strftime('%Y_%m_%d')) + "_data.csv"


async def main():
    # Discover devices
    devices = await BleakScanner.discover()
    logger = None
    for d in devices:
        if KeyValueCoding.getKey(d.details[0], 'name') == 'Arduino':
            logger = d
            print('Found Arduino')
            break
    if not logger:
        print('Arduino not found')
        return

    address = str(KeyValueCoding.getKey(logger.details[0], 'identifier'))
    async with BleakClient(address, timeout=12.0) as client:
        print("Services:")
        svcs = client.services
        for service in svcs:
            print(service)

        temperatureCharacteristic = "d888a9c3-f3cc-11ed-a05b-0242ac120003"
        humidityCharacteristic = "d888a9c4-f3cc-11ed-a05b-0242ac120003"

        with open(filename, "w", newline='') as f:
            fields = ["Time", "Temperature", "Humidity"]
            writer = csv.writer(f)
            writer.writerow(fields)

            try:
                while True:
                    currentDateAndTime = datetime.now()
                    current_time = currentDateAndTime.strftime('%H:%M')

                    # Read characteristics
                    temperature = await client.read_gatt_char(temperatureCharacteristic)
                    humidity = await client.read_gatt_char(humidityCharacteristic)

                    temperature = int.from_bytes(temperature, byteorder='big')
                    humidity = int.from_bytes(humidity, byteorder='big')

                    print("Temperature:", temperature)
                    print("Humidity:", humidity)

                    #write to the csv file
                    data = [current_time, temperature, humidity]
                    writer.writerow(data)
                    f.flush()  # Ensure data is written to the file

                    await asyncio.sleep(10)
            except KeyboardInterrupt:
                print("Stopping data collection.")


asyncio.run(main())
