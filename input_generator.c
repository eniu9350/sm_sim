#include "input_generator.h"

#include <stdlib.h>
#include <math.h>
#include <stdio.h>


static void shuffle_int(int* data, int n)
{
	int temp;
	int i;
	int k1,k2;

	srand(n);
	for(i=0;i<n/2;i++)	{	//swap n/2 times
		k1 = rand()%n;
		k2 = rand()%n;

		temp = data[k1];
		data[k1] = data[k2];
		data[k2] = temp;
	}

	srand(n/2+5);
	for(i=0;i<n/2;i++)	{	//swap n/2 times
		k1 = rand()%n;
		k2 = rand()%n;

		temp = data[k1];
		data[k1] = data[k2];
		data[k2] = temp;
	}

	srand(n*2+5);
	for(i=0;i<n/2;i++)	{	//swap n/2 times
		k1 = rand()%n;
		k2 = rand()%n;

		temp = data[k1];
		data[k1] = data[k2];
		data[k2] = temp;
	}
}


//return "total" count of integers as durations
void generate_duration(int total, int* t_durations)
{
	duration* d;
	int i,j,k;
	double fsum;
	int nsum;
	int bound;
	int nmod = 5;//mmm: should be auto-adjusted

	d = (duration*)malloc(53*sizeof(duration));

	d[0].t = 1;
	d[0].f = pow(10,5.4);
	d[1].t = 2;
	d[1].f = pow(10,5.9);
	d[2].t = 3;
	d[2].f = pow(10,5.95);

	d[3].t = 4;
	d[3].f = pow(10,6);
	d[4].t = 5;
	d[4].f = pow(10,6);
	d[5].t = 6;
	d[5].f = pow(10,6);
	d[6].t = 7;
	d[6].f = pow(10,6);

	for(i=0;i<50-4;i++)	{
		d[7+i].t = pow(2, 3+i*0.2);
		d[7+i].f = pow(10,6)/pow(22.589, log(d[7+i].t/4));
	}

	for(i=0;i<53;i++)	{
		fsum += d[i].f;
	}

	for(i=0;i<53;i++)	{
		d[i].fint = total*d[i].f/fsum;
		//		printf("[%d]%d,%d\n", i, d[i].t, d[i].fint);
	}

	for(i=0;i<53;i++)	{
		if(d[i].fint == 0)	{
			bound = i;
			break;
		}
	}

	for(i=bound;i<53;i++)	{
		d[i].fint = 1;
		d[i%nmod].fint -= 1;	
		//		printf("[%d]%d,%d\n", i, d[i].t, d[i].fint);
	}


	nsum = 0;
	for(i=0;i<53;i++)	{
		nsum+=d[i].fint;
	}

	//distribute the residue
	for(i=0;i<total-nsum;i++)	{
		d[i%nmod].fint += 1;
	}


	//flat representation of durations
	k = 0;
	for(i=0;i<53;i++)	{
		for(j=0;j<d[i].fint;j++)	{
			t_durations[k++] = d[i].t;
		}
	}

	if(total!=k)	{
		printf("error before shuffling in duration generation process.\n");
	}

	//shuffle
	shuffle_int(t_durations, total);


	/*
		 for(i=0;i<total;i++)	{
		 printf("%d ", t_durations[i]);
		 if(i%30==0)	{
		 printf("\n");
		 }
		 }
		 */


	/*
		 nsum = 0;
		 for(i=0;i<53;i++)	{
		 nsum+=d[i].fint;
		 printf("[%d]%d,%d\n", i, d[i].t, d[i].fint);
		 }
		 printf("nsum---%d\n", nsum);
		 */
}

void genenerate_switching_chid(int nclients, int nswitchings, int nchannel, int* chids)
{
	int i,j;
	int r;	//temp for random number

	//flag: 1.switch up; 2.switch down; 3. switch non-linearly;
	int* flag;
	//int* flag_flat;

	int ntotal;
	int nlinear;
	int nup;
	int ndown;
	int nnonlinear;

	ntotal = nclients*(nswitchings-1);
	nlinear = ntotal*0.62;
	nup = ntotal*0.43;
	ndown = nlinear - nup;
	nnonlinear = ntotal - nlinear;

	//set flag
	flag = (int*)malloc(ntotal*sizeof(int));

	//flag_flat = (int*)malloc(ntotal*sizeof(int));

	for(i=0;i<nup;i++)	{	//set up flag
		flag[i] = 1;	
	}
	for(;i<nup+ndown;i++)	{	//set down flag
		flag[i] = 2;
	}
	for(;i<nup+ndown+nnonlinear;i++)	{	//set nonlinear flag
		flag[i] = 3;
	}


	shuffle_int(flag, ntotal);

	/*
	for(i=0;i<ntotal;i++)	{
		printf("flag_flat[%d] = %d ", i, flag_flat[i]);
	}
	*/

	/*
	for(i=0;i<nclients;i++)	{
		for(j=0;j<nswitchings-1;j++)	{
			printf("i=%d,j=%d,i*(nswitchings-1)+j=%d\n", i, j, i*(nswitchings-1)+j);
			//flag[i][j] = flag_flat[i*(nswitchings-1)+j];
		}
	}
	*/
		
	//set init channel id
	srand(nclients*nswitchings);
	for(i=0;i<nclients;i++)	{
		chids[i*(nswitchings)+0] = rand()%nchannel+1;
	}
	

	for(i=0;i<nclients;i++)	{
		for(j=0;j<nswitchings-1;j++)	{
			if(flag[i*(nswitchings-1)+j] == 1)	{
			chids[i*(nswitchings)+j+1] = chids[i*nswitchings+j]+1;
			} else if(flag[i*(nswitchings-1)+j] == 2)	{
			chids[i*nswitchings+j+1] = chids[i*nswitchings+j]-1;
			}	else	{	//non-linear
				srand(chids[i*nswitchings+j]);
				while(1)	{
					r = rand()%nchannel+1;
					if(r!=chids[i*nswitchings+j])	{
						break;
					}
				}
				chids[i*nswitchings+j+1] = r;
			}
		}
	}
}


sm_client* generate_input_clients_simple_1(int n, int nchannels)
{
	sm_client* clients;
	int i,j;
	int nswitchings;	//mmm: should be renamed to nchids!!!
	long arrival;	//same arrival time for each client
	int* durations;
	int* chids;

	nswitchings = 50;
	arrival = 1;

	//generate durations
	durations = (int*)malloc(n*nswitchings*sizeof(int));
	generate_duration(n*nswitchings, durations);	

	clients = (sm_client*)malloc(n*sizeof(sm_client));

	for(i=0;i<n;i++)	{
		clients[i].id = i;
		clients[i].state = CLIENT_STATE_WORKING;
		clients[i].plan = (sm_client_plan*)malloc(sizeof(sm_client_plan));
		clients[i].plan->arrival = arrival;
		clients[i].plan->nswitchings = nswitchings;
		clients[i].plan->switchings = (sm_client_plan_switching_entry**)malloc(nswitchings*sizeof(sm_client_plan_switching_entry*));
		for(j=0;j<nswitchings;j++)	{
			clients[i].plan->switchings[j] = (sm_client_plan_switching_entry*)malloc(sizeof(sm_client_plan_switching_entry));
			clients[i].plan->switchings[j]->duration = durations[i*nswitchings+j];
//			printf("genduration=%ld\n", durations[i*nswitchings+j]);
		}
	}

	//generate chids
	chids = (int*)malloc(n*nswitchings*sizeof(int));
	genenerate_switching_chid(n, nswitchings, nchannels, chids);
	for(i=0;i<n;i++)	{
		for(j=0;j<nswitchings;j++)	{
			clients[i].plan->switchings[j]->chid = chids[i*nswitchings+j];
		}
	}
	printf("3\n");

	return clients;

}
