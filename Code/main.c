#include <avr/io.h> 
#include <util/delay.h>
#include <avr/eeprom.h>
#include <string.h>
#define lcd PORTB
#define en  PB2
#define rw  PB1
#define rs  PB0
 
void send_a_command(unsigned char command);
void send_a_character(unsigned char character);
void send_a_string(unsigned char *str);
void password_check();
uint16_t EEMEM count1 = 0;
uint16_t EEMEM count2 = 0;
uint16_t EEMEM count3 = 0;
uint16_t EEMEM count4 = 0;
int main(void)
{
	unsigned char flag = 1;
    DDRB = 0xFF; //putting portB and portD as output pins
    DDRD = 0xFF;
    DDRA = 0x00; //Taking portA as input.
	DDRC = 0xFF;
    PORTA = 0xFF;  
	
    int16_t COUNTA; // party1 votes storing memory
    char SHOWA [16]; // party1 votes displaying character on LCD
    int16_t COUNTB;  // party2 votes storing memory
    char SHOWB [16]; // party2 votes displaying character on LCD
    int16_t COUNTC; // party3 votes storing memory
    char SHOWC [16]; // party3 votes displaying character on LCD
	int16_t SUM = 0;
	char SHOWSUM [16];
    int16_t COUNTD; // party4 votes storing memory
    char SHOWD [16]; //party4 votes displaying character on LCD
	COUNTA = eeprom_read_word(&count1); //reading from eeprom
	COUNTB = eeprom_read_word(&count2);
	COUNTC = eeprom_read_word(&count3);
	COUNTD = eeprom_read_word(&count4);
    /*send_a_command(0x01);  //Clear Screen 0x01 = 00000001
    _delay_ms(50);
    send_a_command(0x38); //telling lcd we are using 8bit command /data mode
    _delay_ms(50);
    send_a_command(0b00001111); //LCD SCREEN ON and courser blinking*/
	send_a_command(0x02);
	send_a_command(0x28);
	send_a_command(0x06);
	send_a_command(0x0C);
               
    while(1)
    {	
		send_a_command(0x80);
		send_a_string("Electronic");
		send_a_command(0x80 + 0x40);
		send_a_string("Voting Machine");
		while(bit_is_set(PINA,3))
		{	send_a_command(0x80);
			send_a_string("Electronic");
			send_a_command(0x80 + 0x40);
			send_a_string("Voting Machine");
			flag = 1;
			PORTD = 0x0d;
		if(bit_is_clear(PINA,0))
		{
			while(bit_is_clear(PINA,0));
			eeprom_write_word(&count4, COUNTD);
			send_a_command(0x01);
			send_a_command(0x80);
			send_a_string("A=");
			send_a_command(0x80 + 2);
			itoa(COUNTA,SHOWA,10);
			send_a_string(SHOWA);
			send_a_command(0x80 + 8); // shifting cursor  to 9th shell
			send_a_string ("B="); // displaying person2name
			send_a_command(0x80 + 10); //shifting cursor  to 11th shell
			itoa(COUNTB,SHOWB,10); //command for putting variable number in LCD(variable number, in which character to replace, which base is variable(ten here as we are counting number in base10))
			send_a_string(SHOWB);  //telling the display to show character(replaced by variable number) of second person after positioning the courser on LCD.
			send_a_command(0x80 + 0x40 + 0); // shifting cursor  to 1st  shell  of second line
			send_a_string ("C="); // displaying person3 name
			send_a_command(0x80 + 0x40 + 2); //shifting cursor  to 3rd  shell  of second line
			itoa(COUNTC,SHOWC,10);
			send_a_string(SHOWC);
			send_a_command(0x80 + 0x40 + 8); // shifting cursor  to 9th   shell  of second line
            send_a_string ("D="); //displaying person4name
            send_a_command(0x80 + 0x40 + 10); //shifting cursor  to 11th   shell  of second line
			itoa(COUNTD,SHOWD,10);
			send_a_string(SHOWD);
			_delay_ms(2000);
			send_a_command(0x01);
		}
		if(bit_is_clear(PINA,1))
		{
			while(bit_is_clear(PINA,1));
			send_a_command(0x01);
			send_a_string ("Total Votes = "); // displaying person1 name
			send_a_command(0x80 + 0x40);	// shifting cursor  to 3rd shell
			SUM = COUNTA + COUNTB + COUNTC + COUNTD;
			itoa(SUM,SHOWSUM,10);
			send_a_string(SHOWSUM);
			_delay_ms(2000);
			send_a_command(0x01);
		}
		if(bit_is_clear(PINA,2))
		{
			while(bit_is_clear(PINA,2));
			send_a_command(0x01);
			send_a_string("Enter Password");
			password_check();
			COUNTA=COUNTB=COUNTC=COUNTD=0;
			eeprom_write_word(&count1, COUNTA);
			eeprom_write_word(&count2, COUNTB);
			eeprom_write_word(&count3, COUNTC);
			eeprom_write_word(&count4, COUNTD);
			send_a_command(0x01);
			send_a_command(0x80);
			send_a_string("The System has");
			send_a_command(0x80 + 0x40);
			send_a_string("been Reset");
			_delay_ms(2000);
			send_a_command(0x01);
		}
		}
		send_a_command(0x01);
		
		while(flag == 1)
		{
		send_a_command(0x80);
		send_a_string("Please Cast your");
		send_a_command(0x80 + 0x40);
		send_a_string("Vote");
        /*send_a_string ("Total Votes = "); // displaying person1 name
        send_a_command(0x80 + 0x40);	// shifting cursor  to 3rd shell
        SUM = COUNTA + COUNTB + COUNTC + COUNTD;
		itoa(SUM,SHOWSUM,10);
		send_a_string(SHOWSUM);
		
		itoa(COUNTA,SHOWA,10); // command for putting variable number in LCD(variable number, in which character to replace, which base is variable(ten here as we are counting number in base10))
        send_a_string(SHOWA); //telling the display to show character(replaced by variable number) of first person after positioning the courser on LCD.
        send_a_command(0x80 + 8); // shifting cursor  to 9th shell
        send_a_string ("B="); // displaying person2name
        send_a_command(0x80 + 10); //shifting cursor  to 11th shell
        itoa(COUNTB,SHOWB,10); //command for putting variable number in LCD(variable number, in which character to replace, which base is variable(ten here as we are counting number in base10))
        send_a_string(SHOWB);  //telling the display to show character(replaced by variable number) of second person after positioning the courser on LCD.
        send_a_command(0x80 + 0x40 + 0); // shifting cursor  to 1st  shell  of second line
        send_a_string ("C="); // displaying person3 name
        send_a_command(0x80 + 0x40 + 2); //shifting cursor  to 3rd  shell  of second line
        itoa(COUNTC,SHOWC,10);
        send_a_string(SHOWC);
        //send_a_command(0x80 + 0x40 + 8); // shifting cursor  to 9th   shell  of second line
        //send_a_string ("D="); //displaying person4name
        //send_a_command(0x80 + 0x40 + 10); //shifting cursor  to 11th   shell  of second line
        itoa(COUNTD,SHOWD,10);
        send_a_string(SHOWD);*/
        send_a_command(0x80); // shifting cursor  to 0th position
		PORTD = 0x00;
        if (bit_is_clear(PINA,4))//when button one  is pressed
        {	
			while(bit_is_clear(PINA,4));
            COUNTA++;// increment the vote memory of first person by one
			eeprom_write_word(&count1, COUNTA);
			PORTC = 0x01;
			send_a_command(0x01);
			send_a_command(0x80);
			send_a_string("Your Vote has");
			send_a_command(0x80 + 0x40);
			send_a_string("been Recorded");
			_delay_ms(1000);
			PORTC = 0x00;
			send_a_command(0x01);
			flag = 0;
        }
        if (bit_is_clear(PINA,5)) // when button 2  is pressed
        {
			while(bit_is_clear(PINA,5));
            COUNTB++; // increment the vote memory of second person by one
			eeprom_write_word(&count2, COUNTB);
			PORTC = 0x01;
			send_a_command(0x01);
			send_a_command(0x80);
			send_a_string("Your Vote has");
			send_a_command(0x80 + 0x40);
			send_a_string("been Recorded");
			_delay_ms(1000);
			PORTC = 0x00;
			send_a_command(0x01);
			flag = 0;
        }
        if (bit_is_clear(PINA,6)) // when button 3  is pressed
        {	
			while(bit_is_clear(PINA,6));
            COUNTC++; // increment the vote memory of third person by one
			eeprom_write_word(&count3, COUNTC);
			PORTC = 0x01;
			send_a_command(0x01);
			send_a_command(0x80);
			send_a_string("Your Vote has");
			send_a_command(0x80 + 0x40);
			send_a_string("been Recorded");
			_delay_ms(1000);
			PORTC = 0x00;
			send_a_command(0x01);
			flag = 0;
        }
        if (bit_is_clear(PINA,7)) // when button 4  is pressed
        {
			while(bit_is_clear(PINA,7));
            COUNTD++; // increment the vote memory of fourth person by one
			eeprom_write_word(&count4, COUNTD);
			PORTC = 0x01;
			send_a_command(0x01);
			send_a_command(0x80);
			send_a_string("Your Vote has");
			send_a_command(0x80 + 0x40);
			send_a_string("been Recorded");
			_delay_ms(1000);
			PORTC = 0x00;
			send_a_command(0x01);
			flag = 0;
		}
			
		/*PORTD = 0x0d;
		if(bit_is_clear(PINA,0))
		{
			while(bit_is_clear(PINA,0));
			eeprom_write_word(&count4, COUNTD);
			send_a_command(0x01);
			send_a_command(0x80);
			send_a_string("A=");
			send_a_command(0x80 + 2);
			itoa(COUNTA,SHOWA,10);
			send_a_string(SHOWA);
			send_a_command(0x80 + 8); // shifting cursor  to 9th shell
			send_a_string ("B="); // displaying person2name
			send_a_command(0x80 + 10); //shifting cursor  to 11th shell
			itoa(COUNTB,SHOWB,10); //command for putting variable number in LCD(variable number, in which character to replace, which base is variable(ten here as we are counting number in base10))
			send_a_string(SHOWB);  //telling the display to show character(replaced by variable number) of second person after positioning the courser on LCD.
			send_a_command(0x80 + 0x40 + 0); // shifting cursor  to 1st  shell  of second line
			send_a_string ("C="); // displaying person3 name
			send_a_command(0x80 + 0x40 + 2); //shifting cursor  to 3rd  shell  of second line
			itoa(COUNTC,SHOWC,10);
			send_a_string(SHOWC);
			send_a_command(0x80 + 0x40 + 8); // shifting cursor  to 9th   shell  of second line
            send_a_string ("D="); //displaying person4name
            send_a_command(0x80 + 0x40 + 10); //shifting cursor  to 11th   shell  of second line
			itoa(COUNTD,SHOWD,10);
			send_a_string(SHOWD);
			_delay_ms(15000);
			send_a_command(0x01);
		}
		if(bit_is_clear(PINA,1))
		{
			while(bit_is_clear(PINA,1));
			send_a_command(0x01);
			send_a_string ("Total Votes = "); // displaying person1 name
			send_a_command(0x80 + 0x40);	// shifting cursor  to 3rd shell
			SUM = COUNTA + COUNTB + COUNTC + COUNTD;
			itoa(SUM,SHOWSUM,10);
			send_a_string(SHOWSUM);
			_delay_ms(8000);
			send_a_command(0x01);
		}
		if(bit_is_clear(PINA,2))
		{
			while(bit_is_clear(PINA,2));
			COUNTA=COUNTB=COUNTC=COUNTD=0;
			eeprom_write_word(&count1, COUNTA);
			eeprom_write_word(&count2, COUNTB);
			eeprom_write_word(&count3, COUNTC);
			eeprom_write_word(&count4, COUNTD);
			send_a_command(0x01);
			send_a_command(0x80);
			send_a_string("The System has");
			send_a_command(0x80 + 0x40);
			send_a_string("been Reset");
			_delay_ms(8000);
			send_a_command(0x01);
		}*/
			/*send_a_string("Winning Party is");
			send_a_command(0x80 + 0x40);
			if(COUNTA>COUNTB&&COUNTA>COUNTC)
			{
				send_a_string("A");
			}
			else if(COUNTB>COUNTC)
			{
				send_a_string("B");
			}
			else 
			{
				send_a_string("C");
			}
			_delay_ms(10000);
			
			COUNTA=COUNTB=COUNTC=0;
			send_a_command(0x01);
			send_a_command(0x80);*/
        
		
        /*if (bit_is_clear(PINA,4)) // when button 5  is pressed
        {
            COUNTA=COUNTB=COUNTC=COUNTD=0; //clearing vote memory of all persons.
        }*/
		
	}
    }   
	return 0;
    }
 
    /*void send_a_command(unsigned char command)
    {
        PORTB = command;
        PORTD &= ~ (1<<RS); //putting 0 in RS to tell lcd we are sending command
        PORTD |= 1<<E; //telling lcd to receive command /data at the port
        _delay_ms(50);
        PORTD &= ~ (1<<E); //telling lcd we completed sending data
        PORTB = 0;
    }
 
    void send_a_character(unsigned char character)
    {
        PORTB = character;
        PORTD |= 1<<RS; //telling LCD we are sending data not commands
        PORTD |= 1<<E; //telling LCD to start receiving command/data
        _delay_ms(50);
        PORTD &= ~ (1<<E); //telling lcd we completed sending data/command
        PORTB = 0;
    }   
	void send_a_string(unsigned char *str)
	{
		char i=0;
		while(str[i]!='\0')
		{
			send_a_character(str[i]);
			i++;
		}
	}*/
	void send_a_command(unsigned char command)
	{
		lcd = command & 0xF0;//send higher bit
		lcd &= ~(1<<rs); //rs =0
		lcd &= ~(1<<rw);//rw =0
		lcd |=(1<<en);//en =1
		_delay_ms(1);
		lcd &= ~(1<<en);//en =0
		_delay_ms(1);
		lcd = (command<<4) & 0xF0;
		//send lower bit
		lcd &= ~(1<<rs); //rs =0
		lcd &= ~(1<<rw); //rw =0
		lcd |=(1<<en); //en =1
		_delay_ms(1);
		lcd &= ~(1<<en); //en =0
		_delay_ms(1);
	}
	void send_a_character(unsigned char character)
	{
		lcd = character & 0xF0;//send higher bit
		lcd |= (1<<rs); //rs =1
		lcd &= ~(1<<rw); //rw =0
		lcd |=(1<<en); //en =1
		_delay_ms(1);
		lcd &= ~(1<<en); //en =0
		_delay_ms(1);
		lcd =(character<<4) & 0xF0;
		//send lower bit
		lcd |= (1<<rs); //rs =1
		lcd &= ~(1<<rw); //rw =0
		lcd |=(1<<en); //en =1
		_delay_ms(1);
		lcd &= ~(1<<en); //en =0
		_delay_ms(1);
	}
	void send_a_string(unsigned char *str)
	{
		char i=0;
		while(str[i]!='\0')
		{
			send_a_character(str[i]);
			i++;
		}
	}
	void password_check()
	{
		unsigned char pass = 0x00;
		unsigned char count = 0, count1 = 0;
		unsigned char read_value = 0x00;
		char password_set[4] = "1234", password_entered[4];
		while(pass==0x00)
		{
			
			while(count<4)
			{
				PORTD = 0x0e;
				_delay_ms(5); 
				if(bit_is_clear(PINA,0)){while(bit_is_clear(PINA,0)); read_value = '0';}
				if(bit_is_clear(PINA,1)){while(bit_is_clear(PINA,1)); read_value = '1';}
				if(bit_is_clear(PINA,2)){while(bit_is_clear(PINA,2)); read_value = '2';}
				if(bit_is_clear(PINA,3)){while(bit_is_clear(PINA,3)); read_value = '3';}
				PORTD = 0X0d;
				_delay_ms(5);
				if(bit_is_clear(PINA,0)){while(bit_is_clear(PINA,0)); read_value = '4';}
				if(bit_is_clear(PINA,1)){while(bit_is_clear(PINA,1)); read_value = '5';}
				if(bit_is_clear(PINA,2)){while(bit_is_clear(PINA,2)); read_value = '6';}
				if(bit_is_clear(PINA,3)){while(bit_is_clear(PINA,3)); read_value = '7';}
				PORTD = 0X0b;
				_delay_ms(5);
				if(bit_is_clear(PINA,0)){while(bit_is_clear(PINA,0)); read_value = '8';}
				if(bit_is_clear(PINA,1)){while(bit_is_clear(PINA,1)); read_value = '9';}
				if(bit_is_clear(PINA,2)){while(bit_is_clear(PINA,2)); read_value = 'A';}
				if(bit_is_clear(PINA,3)){while(bit_is_clear(PINA,3)); read_value = 'B';}
				PORTD = 0X07;
				_delay_ms(5);
				if(bit_is_clear(PINA,0)){while(bit_is_clear(PINA,0)); read_value = 'C';}
				if(bit_is_clear(PINA,1)){while(bit_is_clear(PINA,1)); read_value = 'D';}
				if(bit_is_clear(PINA,2)){while(bit_is_clear(PINA,2)); read_value = 'E';}
				if(bit_is_clear(PINA,3)){while(bit_is_clear(PINA,3)); read_value = 'F';}
				
				if(read_value)
				{
					password_entered[count] = read_value;
					send_a_command(0xC0 + count);
					send_a_string("*");
					count++;
					read_value = 0x00;
				}
				else
				{
					;
				}
				//_delay_ms(200);
			}
			count = 0;
			send_a_command(0x01);
			//if((strcmp(password_set,password_entered)))
			if(password_set[0]==password_entered[0]&&password_set[1]==password_entered[1]&&password_set[2]==password_entered[2]&&password_set[3]==password_entered[3])
			{
				send_a_string("Correct Password");
				pass = 1;
			}
			else
			{
				send_a_string("Wrong Password");
				_delay_ms(500);
				_delay_ms(500);
				_delay_ms(500);
				_delay_ms(500);
				send_a_command(0x01);
				send_a_string("Reenter Password");
				count1++;
				if(count1 > 2)
				{
					send_a_command(0x01);
					send_a_command(0x80);
					send_a_string("EVM is Locked");
					while(1);
				
				}
				else
				{
					;
				}
			}
		}
		_delay_ms(500);
		_delay_ms(500);
		_delay_ms(500);
		_delay_ms(500);
		send_a_command(0x01);
	}
			
				
		
	
