#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_LEN 128
int diskN = 0;

struct diskReq{
	int arrivalTime;
	int blockNo;
	int servedTime; 
};

struct node
{
	struct diskReq * req;
	struct node * next;
};

struct node * head = NULL;
struct node * tail = NULL;
void queue(struct diskReq * req)
{
	struct node * tmp = (struct node *) malloc(sizeof(struct node));
	tmp->req = req;
	tmp->next = NULL;
	if(head == NULL){
		head = tmp;
		tail = tmp;
	}
	else
	{
		tail->next = tmp;
		tail = tmp;
	}
}

struct diskReq * dequeue()
{
	if(head == NULL)
		return NULL;
	
	struct diskReq * r = head->req;

	struct node * tmp = head;
	head = head->next;
	free(tmp);
	if(head == NULL)
	{		
		tail = NULL;
	}
	
	return r;
}

void printQueue()
{
	struct node * temp = head;
	while(temp != NULL)
	{
		printf("%d %d\n", temp->req->arrivalTime, temp->req->blockNo );
		temp = temp->next;
	}
}

void resetQueue()
{
	struct node * temp = head;

	while(temp != NULL)
	{
		temp->req->servedTime = -1;
		temp = temp->next;
	}
}

double getMean()
{
	double totalW = 0;
	struct node * temp = head;
	int count = 0;
	while(temp != NULL)
	{
		totalW += temp->req->servedTime - temp->req->arrivalTime;
		temp = temp->next;
		count++;
	}
	return totalW/ (double) count;
}

double getStd()
{
	double mean = getMean();
	double variance = 0;
	struct node * temp = head;
	int count = 0;
	while(temp != NULL)
	{
	 	variance += pow((mean - (temp->req->servedTime - temp->req->arrivalTime)), 2);
		temp = temp->next;
		count++;
	}
	variance = variance / (count - 1);
	//printf("%f %d\n", variance, count);
	return sqrt(variance);

}

int allServed()
{
	struct node * temp = head;
	while(temp != NULL)
	{
		if(temp->req->servedTime == -1)
			return 0;
		temp = temp->next;
	}
	return 1;
}

void FCFS()
{
	
	int currentDisk = 1;
	int time = 0;
	struct node * temp = head;
	while(temp != NULL)
	{
		if(time < temp->req->arrivalTime)
		{
			time++;
		}
		else
		{
			struct diskReq * req = temp->req;
			temp = temp->next;

//			printf("%d ", req->blockNo );
			req->servedTime = time;
			//printf("served req d: %d arrived: %d at: %d\n", req->blockNo, req->arrivalTime, time );
			//printf("%d\n",head->req->blockNo );
			time += abs(req->blockNo - currentDisk);
			currentDisk = req->blockNo;
		}


	}	

	//printf("\n");

	printf("FCFS\t:%d %f %f\n", time, getMean(), getStd());
}

void SSTF()
{
	//printQueue();
	int time = 0;
	int currentDisk = 1;
	//struct node * temp;
	//printQueue();
	while(1)
	{
		//printf("\ntime: %d\n", time);
		struct diskReq * nextReq = NULL;
		struct node * temp = head;
		//temp = head;
		//decide the next request to be served		
		//printf("%d %d\n", head->req->blockNo, temp->req->blockNo);
		while(temp != NULL)
		{
			//printf("aaaa	%d\n", temp != NULL);
			struct diskReq *req = temp->req;
			//printf("Worked %d  -  %d\n", req->arrivalTime, time );
			if(req->arrivalTime <= time)
			{				
				//printf("next is null: %d not served: %d\n", nextReq==NULL, req->servedTime == -1);
				if(req->servedTime == -1 && (nextReq == NULL || abs(req->blockNo - currentDisk) < abs(nextReq->blockNo - currentDisk)))
				{
					nextReq = req;
					//temp = temp->next;
				}
			}
			else
			{
				//printf("next not found: %d\n", nextReq== NULL);
				if(nextReq == NULL)
					time++;
				break;
			}


			temp = temp->next;
		}

		if(nextReq == NULL)
		{
			if(allServed())
				break;
			else
				continue;
		}



		//printf("%d ", nextReq->blockNo);
		nextReq->servedTime = time;
		time += abs(nextReq->blockNo - currentDisk);
		//printf("%d\n",head->req->blockNo );
		currentDisk = nextReq->blockNo;



	}

	//printf("\n");

	printf("SSTF\t:%d %f %f\n", time, getMean(), getStd());
}

void LOOK()
{

	int time = 0;
	int currentDisk = 1;
	//struct node * temp;
	//1 to the right, -1 to the left
	int dir = 1;
/*
	int diskArr[diskN];

	int i;
	//1 for waiting for request
	for(i = 0;i < diskN; i++)
		diskArr[i] = 0;
*/	



	//printQueue();
	int dirChangeCount = 0;
	while(1)
	{

		struct node * temp = head;
		struct diskReq * nextReq = NULL;
		while(temp != NULL)
		{

			if(temp->req->arrivalTime <= time)
			{
				//printf("req: %d, time: %d dir: %d\n", temp->req->blockNo, time, dir);
				if(dir == 1)
				{
					if(temp->req->blockNo >= currentDisk)
					{
						if(nextReq == NULL || (temp->req->blockNo - currentDisk) < (nextReq->blockNo - currentDisk))
						{
							if(temp->req->servedTime == -1)
								nextReq = temp->req;
						}
					}
				}
				else if(dir == -1)
				{
					if(temp->req->blockNo <= currentDisk)
					{
						if(nextReq == NULL || (temp->req->blockNo - currentDisk) > (nextReq->blockNo - currentDisk))
						{
							if(temp->req->servedTime == -1)
								nextReq = temp->req;
						}
					}
				}
				
			}
			else
			{
				if(nextReq == NULL)
				{
					if(dirChangeCount < 1)
					{
						dir *= -1;
						dirChangeCount++;
						break;
					}
					//dir *= -1;
					//printf("xx: %d %d\n", time, currentDisk);
					time++;
					dirChangeCount = 0;
				}
				
				break;
				
			}

			
			temp = temp->next;

			if(temp == NULL)
			{
				
				if(allServed())
					break;

				dir *= -1;
				dirChangeCount++;
				if(dirChangeCount > 1)
				{
					if(allServed())
						break;
					else
						continue;
				}
				temp = head;
			}
		}

		//printf("%d %d\n", nextReq == NULL, nextReq->blockNo);
		if(nextReq == NULL)
		{
			if(allServed())
				break;
			else
				continue;
		}	

		//printf("%d ", nextReq->blockNo);
		nextReq->servedTime = time;
		time += abs(nextReq->blockNo - currentDisk);
		dirChangeCount = 0;
		//printf("%d\n",head->req->blockNo );
		currentDisk = nextReq->blockNo;
	}

	//printf("\n");
	printf("LOOK\t:%d %f %f\n", time, getMean(), getStd());

}




void CLOOK()
{

	int time = 0;
	int currentDisk = 1;
	//struct node * temp;
	//1 to the right, -1 to the left
	int dir = 1;
/*
	int diskArr[diskN];

	int i;
	//1 for waiting for request
	for(i = 0;i < diskN; i++)
		diskArr[i] = 0;
*/	



	while(1)
	{

		struct node * temp = head;
		struct diskReq * nextReq = NULL;
		int dirChangeCount = 0;
		while(temp != NULL)
		{
			//printf("%d\n", temp->req->blockNo);
			//printf("\n time: %d\n",time );
			if(temp->req->arrivalTime <= time)
			{
				if(dir == 1)
				{
					if(temp->req->blockNo >= currentDisk)
					{
						if(nextReq == NULL || (temp->req->blockNo - currentDisk) <= (nextReq->blockNo - currentDisk))
						{
							if(temp->req->servedTime == -1)
							{
								//printf("next set: %d %d\n", currentDisk, temp->req->blockNo);
								nextReq = temp->req;
							}
						}
					}
				}				
				else if(dir == -1)
				{
					if(temp->req->blockNo <= currentDisk)
					{
						if(nextReq == NULL || (temp->req->blockNo - currentDisk) > (nextReq->blockNo - currentDisk))
						{
							if(temp->req->servedTime == -1)
								nextReq = temp->req;
						}
					}
				}				
			}
			else
			{
				if(nextReq == NULL)
				{
					struct node * temp2 = head;
					struct node * left = NULL;

					while(temp2 != NULL)
					{
						if(temp2->req->arrivalTime <= time && (left == NULL || temp2->req->blockNo < left->req->blockNo) && temp2->req->servedTime == -1)
							left = temp2;

						temp2 = temp2->next;
					}
					if(left != NULL)
					{

						currentDisk = left->req->blockNo - (currentDisk - left->req->blockNo);
						temp = left;
						break;
					}

					time++;
				}
				break;
				
			}

			temp = temp->next;

			if(temp == NULL)
			{
				/*
				dir *= -1;
				*/
				dirChangeCount++;


				
				if(dirChangeCount > 1)
				{
					if(allServed())
						break;
					else
						continue;
				}
				else if(nextReq == NULL)
				{

					//Find leftmost cylinder which is not served yet
					temp = head;
					int curBlock = head->req->blockNo;
					while(temp != NULL)
					{
						if(temp->req->blockNo < curBlock && temp->req->servedTime == -1)
							curBlock = temp->req->blockNo;

						temp = temp->next;
					}
					

					if(allServed())
						break;
					else if(curBlock == head->req->blockNo && head->req->servedTime != -1)
					{
						time++;
					}


					//time += abs(currentDisk - curBlock);
					//printf("extra added: %d\n",abs(currentDisk - curBlock) );
					currentDisk = curBlock - (currentDisk - curBlock);
					temp = head;
				}
				
			}
		}

		//printf("%d %d\n", nextReq == NULL, nextReq->blockNo);
		if(nextReq == NULL)
		{
			if(allServed())
				break;	
			else 
				continue;
		}

		//printf("served req d: %d arrived: %d at: %d cur: %d\n", nextReq->blockNo, nextReq->arrivalTime, time, currentDisk );
		//printf("%d ", nextReq->blockNo);
		nextReq->servedTime = time;

		time += abs(nextReq->blockNo - currentDisk);
		//printf("%d\n",head->req->blockNo );
		currentDisk = nextReq->blockNo;
		//printf("last current : %d\n", currentDisk);
	}

	//printf("\n");
	printf("CLOOK\t:%d %f %f\n", time, getMean(), getStd());
}



int main(int argc, char ** argv)
{

	if(argc != 3)
		return -1;
	
	diskN = atoi(argv[1]);
	char * fileName = argv[2];

	FILE * file;
	file = fopen(fileName,"r");

	char linebuff[MAX_LEN];
	while(fgets(linebuff, MAX_LEN,file )){
		
		int arrivalTime;
    	arrivalTime = atoi(strtok(linebuff, " "));
    	int diskNo = atoi(strtok(NULL, " "));
    	//printf("%d %d\n", arrivalTime, diskNo);

    	struct diskReq * req = (struct diskReq *) malloc(sizeof(struct diskReq));
    	req->arrivalTime = arrivalTime;
    	req->blockNo = diskNo;
    	req->servedTime = -1;
    	queue(req);
	}

	//printQueue();
	fclose(file);
/*
	dequeue();
	struct diskReq * r = dequeue();
	printf("dequed %d\n", r->blockNo);
	printQueue();
*/
	
	FCFS();
	resetQueue();
	SSTF();
	resetQueue();
	LOOK();
	resetQueue();
	CLOOK();
}

