// Huy Pham
// COP 3502, Spring 2019
// hu079378	

#include <stdio.h>
#include <stdlib.h>
#include "LonelyPartyArray.h"



	LonelyPartyArray *createLonelyPartyArray(int num_fragments, int fragment_length)
	{
		//variables
		int i, max;
		LonelyPartyArray*LPA;

		if ((num_fragments > 0) && (fragment_length > 0))
		{
			max = num_fragments * fragment_length; 
			//allocating for struct
			LPA = malloc(sizeof(LonelyPartyArray));
			//checking if malloc failed
			if (LPA == NULL)
				return NULL;
			
			//intialize memebers of the struct
			LPA->num_fragments = num_fragments;
			LPA->fragment_length = fragment_length;
			LPA->num_active_fragments = 0;
			LPA->size = 0;
			//allocate arrays 
			LPA->fragments = malloc(sizeof(int**) * num_fragments);
			if (LPA->fragments == NULL )
			{
				free(LPA);
				return NULL;
			}
			//intializing fragment_sizes array to zero
			LPA->fragment_sizes = (int*)calloc(num_fragments,sizeof(int));
			if (LPA->fragment_sizes == NULL)
			{
				free(LPA->fragments);
				free(LPA);
				return NULL;
			}
			//intializing fragments array to NULL
			for (i = 0; i < num_fragments; i++)
				LPA->fragments[i] = NULL;
			
			printf("-> A new LonelyPartyArray has emerged from the void. (capacity: %d, fragments: %d)\n", max, num_fragments);
			//return the pointer
			return LPA;
		}
		return NULL;
	}
	
	LonelyPartyArray *destroyLonelyPartyArray(LonelyPartyArray *party)
	{
		//variables
		int i;
		//free values that aren't NULL
		if (party == NULL)
			return NULL;
		
		if (party->fragment_sizes!=NULL)
			free(party->fragment_sizes);
		
		if (party->fragments != NULL)
		{
			//looping through and freeing pointers
			for(i = 0; i < party->num_fragments; i++)
			{
				if(party->fragments[i] != NULL)
					free(party->fragments[i]);
			}
			free(party->fragments);
		}
		free(party);
		printf("-> The LonelyPartyArray has returned to the void.\n");
		return NULL;
	}	
	
	int set(LonelyPartyArray *party, int index, int key)
	{
		//variables
		int i, frag_num, index_in_frag, lowest, highest, invalid_max;
	
		if (party == NULL)
		{
		//NULL pointer
		printf("-> Bloop! NULL pointer detected in set().\n");
		return LPA_FAILURE;
		}
		//calculations
		frag_num = index / party->fragment_length;
		index_in_frag = index % party->fragment_length;
		lowest = party->fragment_length * frag_num;
		highest = ((party->fragment_length * (frag_num + 1)) - 1);
		invalid_max = (party->num_fragments * party->fragment_length - 1);
		//checking if the cell is valid
		if(index < 0 || index > invalid_max)
		{
			printf("-> Bloop! Invalid access in set(). (index: %d, fragment: %d, offset: %d)\n", index, frag_num, index_in_frag);
			return LPA_FAILURE;
		}
		//checking if the fragment was allocated
		if(party->fragments[frag_num] == NULL)
		{    
			party->fragments[frag_num] = malloc(sizeof(int) * party->fragment_length);
			//checking if malloc failed
			if(party->fragments[frag_num] == NULL)
				return LPA_FAILURE;
			
			else
				printf("-> Spawned fragment %d. (capacity: %d, indices: %d..%d)\n", frag_num, party->fragment_length, lowest, highest);
			//update num_active_fragments
			party->num_active_fragments++;	
			//sets the array to UNUSED
			for(i = 0; i < party->fragment_length; i++)
				party->fragments[frag_num][i] = UNUSED;
		}
		//if UNUSED cell
		//insert key
		//update size and fragment_sizes
		if(party->fragments[frag_num][index_in_frag] == UNUSED)
		{
			party->fragments[frag_num][index_in_frag] = key; 
			party->size++;
			party->fragment_sizes[frag_num]++;
		}
		else
			party->fragments[frag_num][index_in_frag] = key; 
		return LPA_SUCCESS;
	}
	
	int get(LonelyPartyArray *party, int index)
	{
		//variables
		int frag_num, index_in_frag, invalid_max;
		
		if (party == NULL)
		{
			printf("-> Bloop! NULL pointer detected in get().\n");
			return LPA_FAILURE;
		}
		frag_num = index / party->fragment_length;
		index_in_frag = index % party->fragment_length;
		invalid_max = (party->num_fragments * party->fragment_length - 1);
		
		if (index < 0 || index > invalid_max)
		{
			printf("-> Bloop! Invalid access in get(). (index: %d, fragment: %d, offset: %d)\n", index, frag_num, index_in_frag);
			return LPA_FAILURE;
		}
			//return UNUSED if fragment was NULL
			//or fragment cell was UNUSED
			if (party->fragments[frag_num] == NULL)	
				return UNUSED;
			
			if (party->fragments[frag_num][index_in_frag] == UNUSED)
				return UNUSED;
			
			return party->fragments[frag_num][index_in_frag];
	}
	
	int delete(LonelyPartyArray *party, int index)
	{
		//variables
		int frag_num, index_in_frag, lowest, highest, invalid_max;
		
		if (party == NULL)
		{
			printf("-> Bloop! NULL pointer detected in delete().\n");
			return LPA_FAILURE;
		}
		frag_num = index / party->fragment_length;
		index_in_frag = index % party->fragment_length;
		lowest = party->fragment_length * frag_num;
		highest = ((party->fragment_length * (frag_num+1)) - 1);
		invalid_max = (party->num_fragments * party->fragment_length - 1);
		
		if (index < 0 || index > invalid_max)
		{
			printf("-> Bloop! Invalid access in delete(). (index: %d, fragment: %d, offset: %d)\n",index , frag_num, index_in_frag);
			return LPA_FAILURE;
		}
		//fail if fragments or frag_size is NULL
		if (party->fragments[frag_num] == NULL)
			return LPA_FAILURE;
		
		if (party->fragment_sizes == NULL)
			return LPA_FAILURE;
		//fail if fragments cell is UNUSED
		if (party->fragments[frag_num][index_in_frag] == UNUSED)
			return LPA_FAILURE;
		//change cell to UNUSED if not UNUSED
		//update size and fragment_sizes
		if (party->fragments[frag_num][index_in_frag] != UNUSED)
		{
			party->fragments[frag_num][index_in_frag] = UNUSED;
			party->size--;
			party->fragment_sizes[frag_num]--;
		}
		//if the fragment is empty deallocate the fragment
		//update num_active_fragments
		if (party->fragment_sizes[frag_num] == 0)
		{
			free(party->fragments[frag_num]);
			party->fragments[frag_num] = NULL;
			party->num_active_fragments--;
			printf("-> Deallocated fragment %d. (capacity: %d, indices: %d..%d)\n", frag_num, party->fragment_length, lowest, highest);
			return LPA_SUCCESS;
		}
		party->fragments[frag_num][index_in_frag] = UNUSED;
		return LPA_SUCCESS;
	}
	
	int containsKey(LonelyPartyArray *party, int key)
	{
		//variables
		int i, j;
		
		if (party = NULL)
		return 0;
			//linear search
		for (i = 0; i < party->num_active_fragments; i ++)
			for (j = 0; i < party->fragment_length; j ++)
				if (party->fragments[i] != NULL)
					if (party->fragments[i][j] == key)
						return 1;
					
		return 0;
			
	}
	
	int isSet(LonelyPartyArray *party, int index)
	{
		//variables
		int frag_num, index_in_frag, invalid_max;
		
		if (party == NULL)
			return 0;
		
		frag_num = index / party->fragment_length;
		index_in_frag = index % party->fragment_length;
		invalid_max = (party->num_fragments * party->fragment_length - 1);
		if (index < 0 || index > invalid_max)
			return 0;
		//return 1 if fragment was allocated
		//or if the cell was UNUSED
		if (party->fragments[frag_num] != NULL)
			if (party->fragments[frag_num][index_in_frag] != UNUSED)
				return 1;
			
		return 0;
	}
	
int printIfValid(LonelyPartyArray *party, int index)
{
	//variables
	int frag_num, index_in_frag, invalid_max;

	if (party == NULL)
	return LPA_FAILURE;

	frag_num = index / party->fragment_length;
	index_in_frag = index % party->fragment_length;
	invalid_max = (party->num_fragments * party->fragment_length - 1);
	
	if (index < 0 || index > invalid_max)
		return LPA_FAILURE;
	if (party->fragments[frag_num] == NULL)
		return LPA_FAILURE;
	if (party->fragments[frag_num][index_in_frag] == UNUSED)
		return LPA_FAILURE;
	else
		//print out the index if not invalid
		printf("%d\n", party->fragments[frag_num][index_in_frag]);

	return LPA_SUCCESS;
}
	
	LonelyPartyArray *resetLonelyPartyArray(LonelyPartyArray *party)
	{
		//variables
		int i, max_array;
		
		if (party == NULL)
		{
			printf("-> Bloop! NULL pointer detected in resetLonelyPartyArray().\n");
			return party;
		}
		max_array = party->num_fragments * party->fragment_length;
		for (i = 0 ; i < party->num_fragments; i++)
		{	
			//loop through and reset the fragment and fragments arrays
			if (party->fragments[i] != NULL)
			{
				free(party->fragments[i]);
				party->fragments[i] = NULL;
				party->fragment_sizes[i] = 0;
			}
		}
		//reset the size and num_active_fragments
		party->size = 0;
		party->num_active_fragments = 0;
		printf("-> The LonelyPartyArray has returned to its nascent state. (capacity: %d, fragments: %d)\n", max_array, party->num_fragments);
		return party;
	}
	
	int getSize(LonelyPartyArray *party)
	{
		if (party == NULL)
			return -1;
		return party->size;
	}
	
	int getCapacity(LonelyPartyArray *party)
	{
		//variables
		int max;
		
		if (party == NULL)
			return -1;
		
		max = party->num_fragments * party->fragment_length;
		return max;	
	}
	
	int getAllocatedCellCount(LonelyPartyArray *party)
	{
		//variables
		int active_frag;
		
		if (party == NULL)
			return -1;
		
		active_frag = party->num_active_fragments * party->fragment_length;
		return active_frag;
	}
	
	long long unsigned int getArraySizeInBytes(LonelyPartyArray *party)
	{
		//variables
		int max;
		
		if (party == NULL)
			return 0;
		
		max = party->num_fragments * party->fragment_length;
		long long unsigned int total = (long long unsigned int) max * (long long unsigned int)sizeof(int);
		return (long long unsigned)total;
	}
	
	long long unsigned int getCurrentSizeInBytes(LonelyPartyArray *party)
	{
		long long unsigned int LPA_pointer, LPA_struct , frag_pointer, frag_size, active_cell, total;
		if (party == NULL)
			return 0;
		//add up using sizeof the LPA pointer, LPA struc, fragment array, 
		//fragments_size array, and active fragments.
		LPA_pointer = (long long unsigned int)sizeof(LonelyPartyArray*);
		LPA_struct = (long long unsigned int)sizeof(LonelyPartyArray);
		frag_pointer =  (long long unsigned int)party->num_fragments * sizeof(int*);
		frag_size = (long long unsigned int)party->num_fragments * sizeof(int);
		active_cell = (long long unsigned int)party->num_active_fragments * party->fragment_length * (long long unsigned int)sizeof(int); 
		total = LPA_pointer + LPA_struct + frag_pointer + frag_size + active_cell;
		return (long long unsigned int)total;
		
	}
	
	double difficultyRating(void)
	{
		return 4.0;
	}
	
	double hoursSpent(void)
	{
		return 24;
	}
	LonelyPartyArray *cloneLonelyPartyArray(LonelyPartyArray *party)
	{
		
		if (party == NULL)
		return NULL;
		//variables
		int i, j, max, NAF, NF, FL, F, FS, S;
		LonelyPartyArray*clone;
		NAF = party-> num_active_fragments;
		NF = party->num_fragments;
		FL = party->fragment_length;
		S = party->size;
		if ((NF > 0) && (FL > 0))
		{
		//allocating for struct
		clone = malloc(sizeof(LonelyPartyArray));
		if (clone == NULL)
			return NULL;
		
		//copying in memebers of the party struct
		clone->num_fragments = FS;
		clone->fragment_length = FL;
		clone->num_active_fragments = NAF;
		clone->size = S;
		//allocate arrays
		//checking if malloc failed
		clone->fragments = malloc(sizeof(int**) * NF);
		if (clone->fragments == NULL )
		{
			free(clone);
			return NULL;
		}
		clone->fragment_sizes = (int*)calloc(NF,sizeof(int));
		if (clone->fragment_sizes == NULL)
		{
			free(clone->fragments);
			free(clone);
			return NULL;
		}
		//allocating space for arrays
		//looping through and copying in the array
		//check if malloc failed
		for (i = 0; i < party->num_fragments; i++)
			for (j = 0; j < NAF; j++)
			{
				clone->fragments[i] = malloc(sizeof(int*));
				if(clone->fragments[i] == NULL)
				{
					free(clone->fragment_sizes);
					free(clone->fragments);
					free(clone);
					return NULL;
				}
			}
		for (i = 0; i < party->num_fragments; i++)
			for (j = 0; j < NAF; j++)
			{
				clone->fragment_sizes[i] = party->fragment_sizes[i];
				if(clone->fragment_sizes[i] == 0)
				{
					clone->fragments[i] = NULL;
					clone->fragments[i] = party->fragments[i];
					clone->fragments[i][j] = party->fragments[i][j];
				}
			}
		max = NF * FL; 
		printf("-> Successfully cloned the LonelyPartyArray. (capacity: %d, fragments: %d)\n", max, NF);
		//return the pointer
		return clone;
		}
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	