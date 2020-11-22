#include <stdio.h>
#include <math.h>
#define MAX 100

static int MAX_WEIGHT = 9;

//1.가성비 가격을 구해주는 함수 -> price/weight = pw
void effective_price_array(int price[], int weight[], int pw[])
{
	for (int i = 0; i < 5; i++)
	{
		pw[i] = price[i] / weight[i];
	}
}

/*                    NUM 2 : BFS  구현                           */
//2. 경계값을 정해주는 함수.
int promising_function(int index, int size, int weight[], int price[], int pw[])
{
	int bound = 0;
	int index_v = index;
	int cur_size = size;

	//우선 , 0-1값으로 넣을 수 있는가 판단. 
	while (index_v < 5)
	{
		if (cur_size - weight[index_v] < 0)
			break;
		bound += price[index_v];
		cur_size -= weight[index_v];
		index_v++;
	}
	int left_size = cur_size;
	if (left_size > 0 && index_v < 5)
	{
		bound += left_size * pw[index_v];
	}
	return bound;
}

//3. 해당 원소의 price의 sum을 구해줌. (Set_bool = {0,1,0,1,0}이런 형식으로 담김)
int sum_set_price(int set_bool[], int price[])
{
	int sum = 0;
	for (int i = 0; i < 5; i++)
	{
		if (set_bool[i] != 0)
			sum += price[i];
	}
	return sum;
}


// max profit 값이 바뀔 때마다 print_array에 {0,1,0,1,0} 등의 값을 저장
//4. 마지막에 저장된 값이 최대값이고, 이때 1만 있는 인덱스의 price만 골라내면됨
void set_array(int set_bool[], int print_array[])
{
	for (int i = 0; i < 5; i++)
		print_array[i] = set_bool[i];
}


//5. 주어진 무게조건 속에서 최대의 가치를 뽑을 수 있는 값(price)들을 모아둠
void print_price(int print_array[], int price[])
{
	printf("Slected price subset = { ");
	for (int i = 0; i < 5; i++)
	{
		if (print_array[i] != 0)
			printf("%d ", price[i]);
	}
	printf("}\n");
}


//[Main1] : DFS 방식 KnapSack 알고리즘 (단, Bound방식으로 제작하였음.)
//(인덱스는 0부터 시작)
void knapSack_DFS(int weight[], int price[], int pw[], int set_bool[], int size, int index, int mp[], int print_array[])
{
	int bound = 0;
	if (index < 5)
	{
		//print_price(print_array, price);
		if (weight[index] <= size)
		{
			int bound = promising_function(index + 1, size - weight[index], weight, price, pw);
			if (mp[0] < sum_set_price(set_bool, price) + price[index] + bound)
			{
				set_bool[index] = 1;
				if (mp[0] < sum_set_price(set_bool, price))
				{
					mp[0] = sum_set_price(set_bool, price);
					//최종적으로 값 복사
					set_array(set_bool, print_array);
				}

				knapSack_DFS(weight, price, pw, set_bool, size - weight[index], index + 1, mp, print_array);
				set_bool[index] = 0;
			}
		}
		//추가하지 않은 경우 
		bound = promising_function(index + 1, size, weight, price, pw);
		if (mp[0] < sum_set_price(set_bool, price) + bound)
		{
			//뽑지 않음을 의미함 
			knapSack_DFS(weight, price, pw, set_bool, size, index + 1, mp, print_array);
		}
	}
	//자연스럽게 재귀호출의 마지막 스택을 쌓고 아래로 내려가버림
}




/*BFS를 위한 큐 구현 -> Breadth Best First Search 방식에서 사용할 예정*/
//1.원형 큐로 구현할 생각임.
void set_array_queue(int queue[])
{
	for (int i = 0; i < 100; i++)
		queue[i] = 0;
}

//2. 원형큐가 비었는지 확인
int isEmpty(int queue[], int front_end[])
{
	//front == end 이면 empty 아니면 full
	if (front_end[0] == front_end[1])
		return 1;
	else
		return 0;
}

//3. 원형큐가 꽉 찼는가 확인
int isFull(int queue[], int front_end[])
{
	int rear = front_end[1];
	if ((rear + 1) % MAX == front_end[0])
		return 1;
	else
		return 0;
}

//4. enqueue 기능
void enqueue(int queue[], int front_end[], int value)
{
	if (isFull(queue, front_end) == 1)
		return;
	else
	{
		front_end[1] = (front_end[1] + 1) % MAX;
		queue[front_end[1]] = value;
	}
}
 
//5. Dequeue 기능 (Deque한 기능을 return해줌)
int dequeue(int queue[], int front_end[])
{
	if (isEmpty(queue, front_end) == 1)
		return -1;
	else
	{
		front_end[0] = (front_end[0] + 1) % MAX;
		int value = queue[front_end[0]];
		queue[front_end[0]] = -1;
		return value;
	}
}




/*                    NUM 2 : BFS  구현                           */
//1. 상태함수트리를 제작할 때 사용하는 reset용 함수 (모두 0으로 만듦)
void reset_binary_array(int binary_array[])
{
	for (int i = 0; i < 5; i++)
		binary_array[i] = 0;
}

//2. 상태함수 변환기 -> 현재 값에서 /2 와 %2의 값을 이용함 
int binary_convert(int value_index, int binary_array[])
{
	//일단 초기화
	reset_binary_array(binary_array);
	int binary = value_index;
	int key = 2;
	int index = 0;

	//0~1
	//2~3
	//4~7
	//8~15
	//15~31
	for (int i = 1; i <= 6; i++)
	{
		if (binary >= pow(key, i - 1) && binary < pow(key, i))
			index = i - 1;
	}
	int cur_index = index - 1;
	for (int k = index; k > 0; k--)
	{
		if (binary > 1)
		{
			int value = binary % 2;
			// 이 값이 짝수면 
			if (value == 0)
				binary_array[k - 1] = 1;
			else
				binary_array[k - 1] = 0;
			binary /= 2;
		}
	}
	return cur_index;
}



//3. 현재 State(노드)의 sum값과 weight를 구해주는 함수. 
void cal_sum(int sum_weight_price[], int binary_array[], int weight[], int price[])
{
	//일단 리셋 
	sum_weight_price[0] = 0;
	sum_weight_price[1] = 0;

	for (int i = 0; i < 5; i++)
	{
		if (binary_array[i] == 1)
		{
			//weight
			sum_weight_price[0] += weight[i];
			//price
			sum_weight_price[1] += price[i];
		}
	}

}

//한계값 (Bound)를 구해주는 함수 (=최대 상한값)
int bound(int sum_weight_price[], int price[], int weight[], int pw[], int index)
{
	int result = 0;
	if (sum_weight_price[0] >= MAX_WEIGHT)
		return 0;
	else
	{
		int index_v = index + 1;
		int totweight = sum_weight_price[0];
		result = sum_weight_price[1];

		while ((index_v < 5) && (totweight + weight[index_v] <= MAX_WEIGHT))
		{
			totweight += weight[index_v];
			result += price[index_v];
			index_v++;
		}
		int left = index_v;
		if (left < 5)
		{
			result += (MAX_WEIGHT - totweight) * pw[left];
		}
		return result;
	}
}


//1부터 시작하는것으로 합시다 -> 어차피 인덱스 값보고 이동하는 것이기 때문.
//maxprofit 은 포인터지만, 1칸 배열 사용예정 (배열 = 포인터) 
//[Main2] : Bredth First Search 방식 KnapSack 알고리즘
int knapSack_BFS(int price[], int weight[], int pw[], int maxprofit[], int print_array2[])
{
	int queue[100] = { 0, };
	int front_end[2] = { 0, };
	front_end[0] = -1;
	front_end[1] = -1;

	set_array_queue(queue);
	int left_binary_array[5] = { 0, };
	int right_binary_array[5] = { 0, };
	int sum_weight_price_left[2] = { 0, };
	int sum_weight_price_right[2] = { 0, };
	int root = 1;
	//차례대로 weight와 price를 계산 값을 담는 함수. 

	enqueue(queue, front_end, root);

	//empty == false 일 때 까지 
	while (isEmpty(queue, front_end) == 0)
	{
		int index = dequeue(queue, front_end);
		int left = index * 2;
		int right = index * 2 + 1;
		int index_left = binary_convert(left, left_binary_array);
		int index_right = binary_convert(right, right_binary_array);
		cal_sum(sum_weight_price_left, left_binary_array, weight, price);
		cal_sum(sum_weight_price_right, right_binary_array, weight, price);

		//일단은 왼쪽 자식부터 설정
		if (sum_weight_price_left[0] <= MAX_WEIGHT && sum_weight_price_left[1] > maxprofit[0])
		{
			maxprofit[0] = sum_weight_price_left[1];
			set_array(left_binary_array, print_array2);
		}

		// price[x] =1 
		if (bound(sum_weight_price_left, price, weight, pw, index_left) > maxprofit[0])
			enqueue(queue, front_end, left);
		//price[x] = 0
		if (bound(sum_weight_price_right, price, weight, pw, index_right) > maxprofit[0])
			enqueue(queue, front_end, right);
	}

	return maxprofit[0];
}








/* Main 3 : Best Frist Search*/

/*힙 정렬에서 사용되는 함수들*/
//0. Heap에서 배열의 값을 바꿀 때 사용하는 함수
void swap_array(int arr1[], int arr2[], int index1, int index2)
{
	int temp = arr1[index1];
	arr1[index1] = arr2[index2];
	arr2[index2] = temp;
}

//1. 힙에서 사용하는 함수 (힙에서 가장 최우선 정렬된 값의 인덱스를 알아내기 위함)
void heap_index_setting(int heap_index[])
{
	for (int i = 0; i < 100; i++)
	{
		heap_index[i] = i;
	}
}

//2. Best First Search를 위한 enqueue (배열 두개를 queue해줌 (value, bound))
void enqueue_bf(int queue[], int queue_bound[], int front_end[], int value, int bound)
{
	if (isFull(queue, front_end) == 1)
		return;
	else
	{
		front_end[1] = (front_end[1] + 1) % MAX;
		queue[front_end[1]] = value;
		queue_bound[front_end[1]] = bound;
	}
}

//3. Best First Search를 위한 dequeue (배열 두개를 dequeue해줌 (value, bound))
int dequeue_bf(int queue[], int queue_bound[], int front_end[])
{
	if (isEmpty(queue, front_end) == 1)
		return -1;
	else
	{
		front_end[0] = (front_end[0] + 1) % MAX;
		int value = queue[front_end[0]];
		queue[front_end[0]] = -1;
		queue_bound[front_end[0]] = 0;
		return value;
	}
}

//4. 힙정렬을 구성하기 위해 힙 세팅
void heap_setting(int heap[])
{
	for (int i = 0; i < 100; i++)
		heap[i] = 0;
}

//5. queue를 바탕으로 heap제작 (MAX_HEAP) 

int make_heap(int heap[], int queue_bound[], int queue[], int front_end[])
{
//조건들
// (a) 힙의 최대값을 pop함
// (b) pop을 하는경우 queue의 마지막 인덱스와 swap하고 pop
	heap_setting(heap);
	int heap_index[100] = { 0, };
	heap_index_setting(heap_index);

	//원소개수를 구해야함
	int count = front_end[1] - front_end[0];
	if (count < 0)
		count += 100;

	int queue_start = front_end[0] + 1;
	//루트 노드 
	heap[0] = queue_bound[queue_start++];
	for (int i = 1; i < count; i++)
	{
		heap[i] = queue_bound[queue_start++];

		int index = i + 1;
		int parent = index / 2;
		while (1)
		{
			//비상용 무한루트 파기함수 
			if (index == 1)
				break;

			if (heap[parent - 1] < heap[index - 1])
			{
				swap_array(heap, heap, parent - 1, index - 1);
				//루트노드의 인덱스 값을 알아내기 위함. 
				swap_array(heap_index, heap_index, parent - 1, index - 1);

				index /= 2;
				parent /= 2;
			}
			else
			{
				break;
			}
		}
	}
	//최대값을 가지는 queue의 index를 리턴해줌. 
	return heap_index[0] + front_end[0] + 1;
}


//6. 최대의 bound를 가지는 queue의 인덱스와 마지막 인덱스를 swap
void queue_change(int queue[], int queue_bound[], int index_change, int front_end[])
{
	//조건: deque하기 전에 이 함수를 먼저 시행해주고 해야함. 
	swap_array(queue, queue, index_change, front_end[0] + 1);
	swap_array(queue_bound, queue_bound, index_change, front_end[0] + 1);
}


//[Main3] : Best First Search 방식 KnapSack 알고리즘
int knapSack_BestFirst(int price[], int weight[], int pw[], int maxprofit[], int print_array2[])
{
	int heap[100] = { 0, };
	int queue[100] = { 0, };
	int queue_bound[100] = { 0, };
	int front_end[2] = { 0, };
	front_end[0] = -1;
	front_end[1] = -1;

	set_array_queue(queue);
	int left_binary_array[5] = { 0, };
	int right_binary_array[5] = { 0, };
	int sum_weight_price_left[2] = { 0, };
	int sum_weight_price_right[2] = { 0, };
	int root = 1;
	//차례대로 weight와 price를 계산 값을 담는 함수. 

	enqueue_bf(queue, queue_bound, front_end, root, 0);

	//empty == false 일 때 까지 
	while (isEmpty(queue, front_end) == 0)
	{
		/*앞의 BFS와 동일한 내용*/
		int index = dequeue_bf(queue, queue_bound, front_end);
		int left = index * 2;
		int right = index * 2 + 1;
		int index_left = binary_convert(left, left_binary_array);
		int index_right = binary_convert(right, right_binary_array);
		cal_sum(sum_weight_price_left, left_binary_array, weight, price);
		cal_sum(sum_weight_price_right, right_binary_array, weight, price);

		//일단은 왼쪽 자식부터 설정
		if (sum_weight_price_left[0] <= MAX_WEIGHT && sum_weight_price_left[1] > maxprofit[0])
		{
			maxprofit[0] = sum_weight_price_left[1];
			set_array(left_binary_array, print_array2);
		}

		// price[x] =1 
		if (bound(sum_weight_price_left, price, weight, pw, index_left) > maxprofit[0])
			enqueue_bf(queue, queue_bound, front_end, left, bound(sum_weight_price_left, price, weight, pw, index_left));
		//price[x] = 0
		if (bound(sum_weight_price_right, price, weight, pw, index_right) > maxprofit[0])
			enqueue_bf(queue, queue_bound, front_end, right, bound(sum_weight_price_right, price, weight, pw, index_right));


		/*dequeue방식 수정*/
		int change = make_heap(heap, queue_bound, queue, front_end);
		queue_change(queue, queue_bound, change, front_end);
	}

	return maxprofit[0];
}


/*실행함수들*/
//가. Depth First Search 알고리즘을 이용한 KnapSack 알고리즘 실행함수
void execute1(int weight[], int price[], int pw[])
{
	printf("\n +++++++++ 1. Depth First Search KnapSack : Start +++++++++\n");
	int set_bool[5] = { 0, };
	int mp[1] = { 0 };
	int print_array[5] = { 0, };
	knapSack_DFS(weight, price, pw, set_bool, MAX_WEIGHT, 0, mp, print_array);
	print_price(print_array, price);
	printf("MaxProfit of Depth First Search(DFS) = %d\n", mp[0]);
	printf(" +++++++++ 1. Depth First Search KnapSack : END +++++++++\n");
}

//나. Bredth First Search 알고리즘을 이용한 KnapSack 알고리즘 실행함수
void execute2(int weight[], int price[], int pw[])
{
	printf("\n +++++++++ 2. Bredth First Search KnapSack : Start +++++++++\n");
	int print_array2[5] = { 0, };
	int maxprofit[1] = { 0, };
	int value_BFS = knapSack_BFS(price, weight, pw, maxprofit, print_array2);
	print_price(print_array2, price);
	printf("MaxProfit of Bredth First Search (BFS) = %d\n", value_BFS);
	printf(" +++++++++ 2. Bredth First Search KnapSack : END +++++++++\n");
}

//다. Best First Search 알고리즘을 이용한 KnapSack 알고리즘 실행함수
void execute3(int weight[], int price[], int pw[])
{
	printf("\n +++++++++ 3. Best First Search KnapSack : Start +++++++++\n");
	int print_array3[5] = { 0, };
	int maxprofit2[1] = { 0, };
	int var3 = knapSack_BestFirst(price, weight, pw, maxprofit2, print_array3);
	print_price(print_array3, price);
	printf("MaxProfit of Best First Search = %d\n", var3);
	printf(" +++++++++ 3. Best First Search KnapSack : END +++++++++\n");
}




int main(void)
{
	int weight[5] = { 2,5,7,3,1 };
	int price[5] = { 20,30,35,12,3 };
	int pw[5] = { 0, };
	effective_price_array(price, weight, pw);
	
	printf("\nKnapSack을 실행하기 위한 무게 조건은 = %d 입니다.\n",MAX_WEIGHT);
	execute1(weight, price, pw);
	execute2(weight, price, pw);
	execute3(weight, price, pw);
	return 1;
}