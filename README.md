 lipobatterytester
# DIY LiPo Battery Capacity Tester

# Intro:

Like all of you, I also have a bunch of salvaged Lithium-Polymer (LiPo) batteries from old laptop battery pack or from old power banks. They all have different rated capacity and we do not know the present capacity of these batteries. There are many capacity tester in market commercially available but i thought why not use our arduino to make a capacity tester. Making it was super fun with lots of troubleshooting but at the end I got it working with good results.

# Supplies:
Perf board or fabricated PCB

arduino Nano

Power resistors 10W - 7.8 Ohm

Shunt Resistors 10W - 0.2 Ohm

OLED screen

filter capacitors - 100nf - (code 103) - to smooth out the input readings

N-channel MOSFET - logic level - SI2302 (SMD)


# Schematic Designing, PCB Layout and Ordering PCB

I have used EasyEda to design the 2-layer PCB.
It’s easy and fun to use and has the footprint libraries of most of the components. If anyone wants i can always post the gerber files for you.

Now we have got the PCB design and it’s time to order the PCB’s. For that, you just have to go to JLCPCB.com (https://jlcpcb.com/), and click on “QUOTE NOW” button.

JLCPCB are also sponsor of this project. JLCPCB (Shenzhen JLC Electronics Co., Ltd.), is the largest PCB prototype enterprise in China and a high-tech manufacturer specializing in quick PCB prototype and small-batch PCB production. You can order a minimum of 5 PCBs for just $2. To get the PCB manufactured, upload the gerber file you downloaded in the last step. Upload the .zip file or you can also drag and drop the gerber files. After uploading the zip file, you’ll see a success message at the bottom if the file is successfully uploaded. You can review the PCB in the Gerber viewer to make sure everything is good. You can view both top and bottom of the PCB. After making sure the PCB looks good, you can now place the order at a reasonable price. You can order 5 PCBs for just $2 plus shipping. To place the order, click on “SAVE TO CART” button. My PCBs took 2 days to get manufactured and arrived within 20 days using standard registered post delivery option. There are fast delivery options also available.PCBs were well packed and the quality was really good.

JLCPCB is now offering SMT assembly service that too at a very reasonable price and after trying it I must say that they are doing excellent work and their craftsmanship is comparable to any other commercial PCB manufacturing and assembly service all over the world. Their backend team is so efficient and through that they will check each and every design and component placement and will inform the customer about required changes in design or about wrong placement and polarity of the components and correct the minor issues at their end itself.

# Working Principle

Capacity of any battery is measured in mAh(milliamp hours) or Ah (Amp Hours) depending on the size of batteries, lead acid battery or lithium ferrous phosphate (LiFePO4) battery capacities are mentioned in Ah while for lithium based small batteries its mentioned in mAh.

Capacity of a battery is the amount of power it can deliver from full charge to full discharge. So for example, a 2200mAh lipo battery means that it can deliver max of 2200mA or 2.2A for 1 hour or 2.2mA for 1000 hours.

Thus for us to calculate the total capacity of a battery we need to calculate the current consumed over a time period.

Total capacity (Q) = amps * hours or Q = mA * 1000 * Hours

Using arduino, calculating current is not possible directly, but we can calculate the voltage using the analog input pins. Then we can get the current (I) using the ohm's law.

Here, we will calculate the voltage drop across the known resistance (shunt) to get the current and use it for calculating the capacity.

V = I * R so I = V / R ( Ohm's Law) - I is current is amps, V is volts , R is resistance in ohms

But, for any battery chemistry the voltage does not remain constant while the battery is discharging, thus the voltage will gradually decrease over the time of discharge. So we need to check the voltage of the battery every second at least to get a decent accuracy of the total capacity. As the voltage will slowing decrease, so will the current.

Thus, we have to calculate the current every second and use that current value to get our capacity of every second.
i1 = voltage drop / shunt resistance -- current at each second
In arduino, time is calculated in milliseconds thus we need to convert this milliseconds to hours.
so 1 millisecond - 1/3600000 hours.
We are calculating the current every second (1000 milliseconds).
thus q1 = current * time (i1 * time passed) - this is the capacity calculated for first second of the discharge process of the battery. Likewise we need to calculate the q2,q3,q4...qn till the end of the discharge process.
for LiPo cell the max voltage is 4.2 volts and minimum safe voltage is 3.2 or 2.9 volts. So we need to calculate the capacity used each second till the voltage reaches to minimum and then add all the individual capacity to get the final total capacity.
Q = q1 + q2 + q3 + q4 + .. + qn.

Here, I have taken shunt resistance of 0.2 ohms.
To limit the current to around 500mA when voltage is max at 4.2V, I have connected in series resistance of 7.8 ohms, thus getting the total resistance of 8 ohms.
R = V / I , R = 4.2 / 0.5 = 8.4 Ohms -- ideally to get 500mA current draw resistance should be 8.4 ohms
so, current @ 4.2V = 4.2/8 = 525 mA (I=V/R) - when the voltage is max and batteries are fully charged.
Current @ 3.2V = 3.2/8 = 400 mA
Also all the resistors are power resistors capable of dissipating power of at least 5 Watts, 10W rated are optimal.
power = V*I = 4.2 * 0.525 = 2.2 Watt - max power to be dissipated by the resistors.
power = V * I = 3.2 * 0.4 = 1.28 Watt
As a side note, if you do not have power resistors you can use 1/2W resistors in parallel. Five 1/2W 40 ohm resistors will give you a effective resistance of 8 ohms and then you can add a suitable shunt resistor in series to them. These 1/2W resistors in parallel will be able to handle the power dissipation of 2.2 watts easily.
