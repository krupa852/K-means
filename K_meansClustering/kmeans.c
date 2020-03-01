#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

#define K 3

typedef struct datapoint
{ 
	int x;
	int y;
	int cluster;
}datapoint;

typedef struct centroid
{
	double x;
	double y;
}centroid;

centroid Centroids[K];
datapoint Datapoint[100];

#define square(x) (double)((x)*(x))
// Calculate the Euclidean distance between two points.
double distance( int x1, int y1, int x2, int y2 )
{
        return sqrt(square((x2 - x1)) + square((y2 - y1)));
}

int getRandom(double x){
	double j = (double) rand() / (double) RAND_MAX;
	return (int)((x)*j);
}

void Initialization(int datacount)
{  
	// Assign each datapoint to a random cluster.
	for(int i=0;i < datacount;i++)
   	{
      		Datapoint[i].cluster = getRandom(K);
   	}
}

void updateCentroid( int datacount )
{
   	int Count[K];

   	for(int i = 0 ; i < K ;i++)
   	{
      		Centroids[i].x = 0.0;
      		Centroids[i].y = 0.0;
      		Count[i] = 0;
   	}

        // Update the centroids value
   	for(int i = 0;i < datacount;i++)
   	{
      		Centroids[Datapoint[i].cluster].x += (double)Datapoint[i].x;
      		Centroids[Datapoint[i].cluster].y += (double)Datapoint[i].y;
      		Count[ Datapoint[i].cluster ]++;
   	}

   	// Update centroids to mean value for respective data samples belonging to same cluster.
   	for(int i=0 ;i < K ;i++ )
   	{
      		if ( Count[i] > 1 )
      		{
         		Centroids[i].x /= (double)Count[i];
         		Centroids[i].y /= (double)Count[i];
      		}
   	}
}

//find  Closestcluster to the given data point
int CloseClustertodatapoint( int idx )
{
        int ClosestCluster;
        double Closest;
        ClosestCluster = 0;
        Closest = distance( (double)Datapoint[idx].x,
                       (double)Datapoint[idx].y,
                        Centroids[ ClosestCluster ].x,
                        Centroids[ ClosestCluster ].y );

  
        for(int i=1;i < K ;i++)
        {
                double EuDistance;
                EuDistance = distance( (double)Datapoint[idx].x,
                           (double)Datapoint[idx].y,
                            Centroids[i].x,
                            Centroids[i].y );

                if(EuDistance < Closest)
                {
                Closest = EuDistance;
                ClosestCluster = i;
                }
        }

        return ClosestCluster;
}


int Assigntoclosestcluster(int datacount)
{
   	int count = 0;

   	for(int i=0 ;i < datacount;i++)
   	{
      		int ClosestCluster = CloseClustertodatapoint(i);

      		if(ClosestCluster != Datapoint[i].cluster)
      		{
         		// Change the cluster parameter to new cluster value.
         		Datapoint[i].cluster = ClosestCluster;
         		count++;
      		}

   	}

   	return count;
}

int main( int argc, char *argv[] )
{
   	//int datasamples;
	
	if(argc!=2)
	{
		printf("Error");
		return 0;
	}
	//srand( time( NULL ) );

	//read from input file
   	FILE *fp = fopen(argv[1], "r");
        int ptcount=0;
        while(!feof(fp))
        {
                fscanf(fp, "%d %d %d\n", &Datapoint[ptcount].x,&Datapoint[ptcount].y,&Datapoint[ptcount].cluster);
                ptcount++;
        }
        fclose(fp);
    
   	Initialization(ptcount);

   	//continue to update centroid until stable centroids.
   	do
   	{
      		updateCentroid(ptcount);
   	} while (Assigntoclosestcluster(ptcount));

   	// print datapoints after cluster assignment.
   	for( int i=0;i<ptcount;i++)
        {
                printf("%d %d %d\n", Datapoint[i].x,Datapoint[i].y,Datapoint[i].cluster);
	}
	for(int i=0;i<K;i++)
	{
		printf("%f %f 3\n",Centroids[i].x,Centroids[i].y);
	}
   	return 0;
}
