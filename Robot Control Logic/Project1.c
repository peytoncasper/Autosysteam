/* helloworld.c for TOPPERS/ATK(OSEK) */
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"

int current_rev, neg_speed;
DeclareTask(Task1);
void user_1ms_isr_type2(void){ /* do nothing */ }
void disp(int row, char *str1, int val1, char *str2,int val2)
{
	display_clear(0);
	display_goto_xy(0, row);
	display_string(str1);
	display_int(val1, 0);
	display_goto_xy(0, row+1);
	display_string(str2);
	display_int(val2, 0);
	display_update();
}

void go_straight(int speed, int goal_rev)
{		
	nxt_motor_set_count(NXT_PORT_A,0);
	nxt_motor_set_count(NXT_PORT_B,0);
	current_rev=nxt_motor_get_count(NXT_PORT_B);
	int revA=0,revB=0;
	float diff;
	while(goal_rev+1!=current_rev)
		{
					revA=nxt_motor_get_count(NXT_PORT_A);
					revB=nxt_motor_get_count(NXT_PORT_B);					
					disp(4,"StraightA:",revA,"StraightB:",revB);
					
					if(revA<revB)
					{
					diff=(revB-revA)/4;
					nxt_motor_set_speed(NXT_PORT_A,speed+diff,1);
					nxt_motor_set_speed(NXT_PORT_B,speed,1);
					}
					else
					{
					diff=(revA-revB)/4;
					nxt_motor_set_speed(NXT_PORT_A,speed,1);
					nxt_motor_set_speed(NXT_PORT_B,speed+diff,1);
					}					
					current_rev=nxt_motor_get_count(NXT_PORT_B);
			//systick_wait_ms(500);
		}
	disp(4,"StraightA:",revA,"StraightB:",revB);
	nxt_motor_set_speed(NXT_PORT_A,0, 1);
	nxt_motor_set_speed(NXT_PORT_B,0, 1);
}

void turn_left(int speed, int goal_rev)
{
	nxt_motor_set_count(NXT_PORT_A,0);
	nxt_motor_set_count(NXT_PORT_B,0);
	neg_speed=speed*(-1);
	current_rev=nxt_motor_get_count(NXT_PORT_A);
		while(goal_rev!=current_rev)
		{
					nxt_motor_set_speed(NXT_PORT_A,speed,1);
					nxt_motor_set_speed(NXT_PORT_B,neg_speed,1);
					current_rev=nxt_motor_get_count(NXT_PORT_A);
		}
	//disp(4,"Left:",goal_rev);
	nxt_motor_set_speed(NXT_PORT_A,0, 1);
	nxt_motor_set_speed(NXT_PORT_B,0, 1);
}

void turn_right(int speed, int goal_rev)
{
	nxt_motor_set_count(NXT_PORT_A,0);
	nxt_motor_set_count(NXT_PORT_B,0);
	neg_speed=speed*(-1);	
	current_rev=nxt_motor_get_count(NXT_PORT_B);
		while(goal_rev!=current_rev)
			{
			nxt_motor_set_speed(NXT_PORT_A,neg_speed,1);
			nxt_motor_set_speed(NXT_PORT_B,speed,1);
			current_rev= nxt_motor_get_count(NXT_PORT_B);
			}
	//disp(4,"Right:",goal_rev);
	nxt_motor_set_speed(NXT_PORT_A,0, 1);
	nxt_motor_set_speed(NXT_PORT_B,0, 1);
}

void stop()
{
	nxt_motor_set_speed(NXT_PORT_A,0, 1);
	nxt_motor_set_speed(NXT_PORT_B,0, 1);
	TerminateTask();
}


TASK(Task1)
{	
	int time_out;
	time_out = systick_get_ms() + 20000;
	//while(systick_get_ms() < time_out)
	//{	

	int x[]={1,2,2,2,2,1,1,0,0,0};
	int y[]={1,1,2,3,4,4,3,3,2,1};
	int x1=0;
	int y1=1;
	int x2,y2;
	int head=3;
	for (int i=0;i<=9;i++)
	{
		x2=x[i];
		y2=y[i];
		if(y1==y2)
		{
			switch(head)
			{
				case 1:	
					if(x2>x1)
					{
					turn_right(40,190);
					turn_right(40,190);
					go_straight(40,720);
					head=3;
					x1=x2;
					}
					else
					{
					go_straight(40,720);
					head=1;
					x1=x2;
					}
				break;
				case 2:
					if(x2>x1)
					{
					turn_right(40,190);
					go_straight(40,720);
					head=3;
					x1=x2;
					}
					else
					{
					turn_left(40,163);     
					go_straight(40,720);
					head=1;
					x1=x2;
					}
				break;
				case 3:	
					if(x2>x1)
					{
					go_straight(40,720);
					head=3;
					x1=x2;
					}
					else
					{
					turn_right(40,190);
					turn_right(40,190);
					go_straight(40,720);
					head=1;
					x1=x2;
					}
				break;
				case 4:
					if(x2>x1)
					{
					turn_left(40,163); 
					go_straight(40,720);
					head=3;
					x1=x2;
					}
					else
					{
					turn_right(40,190);
					go_straight(40,720);
					head=1;
					x1=x2;
					}
				break;
				default:
					//disp(4,"Default for x:",2);
				break;
			}		
		}
		else
		{
			if(x1==x2)
			{
				switch(head)
				{
					case 1:	
						if(y2>y1)
						{
						turn_right(40,190);
						go_straight(40,720);
						head=2;
						y1=y2;
						}
						else
						{
						turn_left(40,163);  
						go_straight(40,720);
						head=4;
						y1=y2;
						}
					break;
					case 2:
						if(y2>y1)
						{
						go_straight(40,720);
						head=2;
						y1=y2;
						}
						else
						{
						turn_right(40,190);
						turn_right(40,190);    
						go_straight(40,720);
						head=4;
						y1=y2;
						}
					break;
					case 3:	
						if(y2>y1)
						{
						turn_left(40,163);  
						go_straight(40,720);
						head=2;
						y1=y2;
						}
						else
						{
						turn_right(40,190);
						go_straight(40,720);
						head=4;
						y1=y2;
						}
					break;
					case 4:
						if(y2>y1)
						{
						turn_right(40,190);
						turn_right(40,190);
						go_straight(40,720);
						head=2;
						y1=y2;
						}
						else
						{
						go_straight(40,720);
						head=4;
						y1=y2;
						}
					break;
					default:
						//disp(4,"Default for y:",2);
					break;
				}
	
			}	
		}
	}

	//go_straight(40,720);
	//turn_right(40,190);					
	//go_straight(40,720);
	//turn_left(40,163);                           
	//go_straight(40,720);
	stop();	


	//disp(3, "  RN: ", time_out);
	//go_straight(70,720);
	//turn_left(40,270);
		
	//disp(3, "  RN: ", 3);
	//go_straight(70,720);
	//disp(3, "  RN: ", 4);
	//turn_left(20,270);
	//disp(3, "  RN: ", 5);	
	//turn_right(20,180);
	//disp(3, "  RN: ", 6);
	//stop();
	
	
	//}
	TerminateTask();

}

