stopper:	add $s0, $zero, $zero, $zero, 0			#$s0=sec1=0
			add $s1, $zero, $zero, $zero, 0			#$s1=sec2=0
			add $a0, $zero, $zero, $zero, 0			#$a0=min1=0
			add $a1, $zero, $zero, $zero, 0			#$a1=min2=0
			add $t0, $zero, $zero, $zero, 9			#$t0=9
			add $t1, $zero, $zero, $zero, 5			#$t1=5
			add $at, $zero, $zero, $zero, 1			#$at=1
			in  $t3, $zero, $zero, $zero, 3			#$t3=IORegister[3] 
			add $s2, $zero, $zero, $zero, 0			#$s2=IORegister[0]
			add $t2, $zero, $zero, $zero, 0			#$t2=0
			add $v0, $zero, $zero, $zero, 0			#time=0x00000000 (0x0000min2min1sec2sec1)
			out $v0, $zero, $zero, $zero, 4			#write to SSD: IORegister[4]=time
			
			in $t2, $zero, $zero, $zero, 0			#$t2=IORegister[0]
			add $t2, $t2, $zero, $zero, 28			#$t2=IORegister[0]+28
Loop:		in $s2, $zero, $zero, $zero, 3			#$s2=IORegister[3]
			branch $zero, $s2, $t3, 1, stopper		#jump to stopper if BTND was pressed
			in $s2, $zero, $zero, $zero, 2			#$s2=IORegister[2]
			and $s2, $s2, $at, $zero, 1				#$s2=LSB(IORegister[2])
			branch $zero, $s2, $at, 0, BTNC			#jump to BTNC if BTNC was pressed
			
			add $s0, $s0, $zero, $zero, 1			#sec1++
			branch $zero, $s0, $t0, 2, Sec2			#jump to Sec2 if sec1>9
			add $v0, $v0, $zero, $zero, 1			#time[sec1]++
Os1:		in $s2, $zero, $zero, $zero, 0			#$s2=IORegister[0]
			branch $zero, $s2, $t2, 3, Os1 			#jump to Os1 if (IORegister[0]<32)			
			out $v0, $zero, $zero, $zero, 4			#write to SSD: IORegister[4]=time
			in $t2, $zero, $zero, $zero, 0			#$t2=IORegister[0]
			add $t2, $t2, $zero, $zero, 28			#$t2=IORegister[0]+28
			branch $zero, $zero, $zero, 0, Loop		#unconditional jump Loop
			
Sec2:		add $s0, $zero, $zero, $zero, 0			#sec1=0
			add $s1, $s1, $zero, $zero, 1			#sec2++
			branch $zero, $s1, $t1, 2, Min1			#jump to Min1 if sec2>5
			sub $v0, $v0, $zero, $zero, 9 			#time[sec1]=mms0
			add $v0, $v0, $zero, $zero, 16			#time[sec2]++ (0x00000010=16)
Os2:		in $s2, $zero, $zero, $zero, 0			#$s2=IORegister[0]
			branch $zero, $s2, $t2, 3, Os2 		    #jump to Os2 if (IORegister[0]<32)
			out $v0, $zero, $zero, $zero, 4			#write to SSD: IORegister[4]=time
			in $t2, $zero, $zero, $zero, 0			#$t2=IORegister[0]
			add $t2, $t2, $zero, $zero, 28			#$t2=IORegister[0]+28
			branch $zero, $zero, $zero, 0, Loop		#unconditional jump Loop
			
Min1:		add $s0, $zero, $zero, $zero, 0			#sec1=0
			add $s1, $zero, $zero, $zero, 0 		#sec2=0
			add $a0, $a0, $zero, $zero, 1			#min1++
			branch $zero, $a0, $t0, 2, Min2			#jump to Min2 if min1>9
			sub $v0, $v0, $zero, $zero, 89 			#time[sec2sec1]=mm00 (0x00000059=89)
			add $v0, $v0, $zero, $zero, 256			#time[min1]++ (0x00000100=256)
Om1:		in $s2, $zero, $zero, $zero, 0			#$s2=IORegister[0]
			branch $zero, $s2, $t2, 3, Om1 			#jump to Om1 if (IORegister[0]<32)
			out $v0, $zero, $zero, $zero, 4			#write to SSD: IORegister[4]=time
			in $t2, $zero, $zero, $zero, 0			#$t2=IORegister[0]
			add $t2, $t2, $zero, $zero, 28			#$t2=IORegister[0]+28
			branch $zero, $zero, $zero, 0, Loop		#unconditional jump Loop
			
Min2:		add $s0, $zero, $zero, $zero, 0			#sec1=0
			add $s1, $zero, $zero, $zero, 0			#sec2=0
			add $a0, $zero, $zero, $zero, 0			#min1=0
			add $a1, $a1, $zero, $zero, 1			#min2++
			branch $zero, $a1, $t1, 2, stopper		#jump to stopper if min2>5
			sra $v0, $v0, $zero, $zero, 12			#$v0=>>12($v0) (0x0000000min2)
			add $v0, $v0, $zero, $zero, 1			#time[min2++]
			sll $v0, $v0, $zero, $zero, 12			#$v0=($v0)12<< (0x0000min2000)
Om2:		in $s2, $zero, $zero, $zero, 0			#$s2=IORegister[0]
			branch $zero, $s2, $t2, 3, Om2 			#jump to Om2 if (IORegister[0]<32)
			out $v0, $zero, $zero, $zero, 4			#write to SSD: IORegister[4]=time
			in $t2, $zero, $zero, $zero, 0			#$t2=IORegister[0]
			add $t2, $t2, $zero, $zero, 28			#$t2=IORegister[0]+28
			branch $zero, $zero, $zero, 0, Loop		#unconditional jump Loop

BTNC:		out $at, $zero, $zero, $zero, 1			#turn on LD0: IORegister[1]=0x0000001
			in $t2, $zero, $zero, $zero, 0			#$t2=IORegister[0]
			add $t2, $t2, $zero, $zero, 150			#$t2=IORegister[0]+(5*32-10=150) 
On:			in $s2, $zero, $zero, $zero, 2			#$s2=IORegister[2]
			and $s2, $s2, $at, $zero, 1				#$s2=LSB(IORegister[2])
			branch $zero, $s2, $at, 1, BTNC2		#jump to BTNC2 if BTNC was pressed again
			in $s2, $zero, $zero, $zero, 0			#$s2=IORegister[0]
			branch $zero, $s2, $t2, 3, On 			#jump to On if (IORegister[0]<5*32=160)
			out $zero, $zero, $zero, $zero, 1		#Turn off LD0: IORegister[1]=0x00000000
			in $t2, $zero, $zero, $zero, 0			#$t2=IORegister[0]
			add $t2, $t2, $zero, $zero, 150			#$t2=IORegister[0]+150
Off:		in $s2, $zero, $zero, $zero, 2			#$s2=IORegister[2]
			and $s2, $s2, $at, $zero, 1				#$s2=LSB(IORegister[2])
			branch $zero, $s2, $at, 1, Loop			#jump to Loop if BTNC was pressed again
			in $s2, $zero, $zero, $zero, 0			#$s2=IORegister[0]
			branch $zero, $s2, $t2, 3, Off 			#jump to Off if (IORegister[0]<5*32=160)
			branch $zero, $zero, $zero, 0, BTNC		#Unconditional jump BTNC
			
BTNC2:		out $zero, $zero, $zero, $zero, 1		#turn off LD0: IORegister[1]=0x00000000
			branch $zero, $zero, $zero, 0, Loop		#unconditional jump Loop
			
			
			
			
		
		
