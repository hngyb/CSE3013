#include "tetris.h"

static struct sigaction act, oact;

int main()
{
	int exit = 0;

	initscr();
	noecho();
	keypad(stdscr, TRUE);

	srand((unsigned int)time(NULL));

	createRankList();

	while (!exit)
	{
		clear();
		switch (menu())
		{
		case MENU_PLAY:
			play();
			break;
		case MENU_RANK:
			rank();
			break;
		case MENU_EXIT:
			exit = 1;
			break;
		default:
			break;
		}
	}

	writeRankFile();
	endwin();
	system("clear");
	return 0;
}

void InitTetris()
{
	int i, j;

	for (j = 0; j < HEIGHT; j++)
		for (i = 0; i < WIDTH; i++)
			field[j][i] = 0;

	nextBlock[0] = rand() % 7;
	nextBlock[1] = rand() % 7;
	nextBlock[2] = rand() % 7;
	blockRotate = 0;
	blockY = -1;
	blockX = WIDTH / 2 - 2;
	score = 0;
	gameOver = 0;
	timed_out = 0;

	DrawOutline();
	DrawField();
	DrawBlockWithFeatures(blockY, blockX, nextBlock[0], blockRotate);
	DrawNextBlock(nextBlock);
	PrintScore(score);
}

void DrawOutline()
{
	int i, j;
	/* 블럭이 떨어지는 공간의 태두리를 그린다.*/
	DrawBox(0, 0, HEIGHT, WIDTH);

	/* 첫 번째 next block을 보여주는 공간의 태두리를 그린다.*/
	move(2, WIDTH + 10);
	printw("NEXT BLOCK");
	DrawBox(3, WIDTH + 10, 4, 8);

	/* 두 번째 next block을 보여주는 공간의 태두리를 그린다.*/
	DrawBox(9, WIDTH + 10, 4, 8);

	/* score를 보여주는 공간의 태두리를 그린다.*/
	move(15, WIDTH + 10);
	printw("SCORE");
	DrawBox(16, WIDTH + 10, 1, 8);
}

int GetCommand()
{
	int command;
	command = wgetch(stdscr);
	switch (command)
	{
	case KEY_UP:
		break;
	case KEY_DOWN:
		break;
	case KEY_LEFT:
		break;
	case KEY_RIGHT:
		break;
	case ' ': /* space key*/
		/*fall block*/
		break;
	case 'q':
	case 'Q':
		command = QUIT;
		break;
	default:
		command = NOTHING;
		break;
	}
	return command;
}

int ProcessCommand(int command)
{
	int ret = 1;
	int drawFlag = 0;
	switch (command)
	{
	case QUIT:
		ret = QUIT;
		break;
	case KEY_UP:
		if ((drawFlag = CheckToMove(field, nextBlock[0], (blockRotate + 1) % 4, blockY, blockX)))
			blockRotate = (blockRotate + 1) % 4;
		break;
	case KEY_DOWN:
		if ((drawFlag = CheckToMove(field, nextBlock[0], blockRotate, blockY + 1, blockX)))
			blockY++;
		break;
	case KEY_RIGHT:
		if ((drawFlag = CheckToMove(field, nextBlock[0], blockRotate, blockY, blockX + 1)))
			blockX++;
		break;
	case KEY_LEFT:
		if ((drawFlag = CheckToMove(field, nextBlock[0], blockRotate, blockY, blockX - 1)))
			blockX--;
		break;
	default:
		break;
	}
	if (drawFlag)
		DrawChange(field, command, nextBlock[0], blockRotate, blockY, blockX);
	return ret;
}

void DrawField()
{
	int i, j;
	for (j = 0; j < HEIGHT; j++)
	{
		move(j + 1, 1);
		for (i = 0; i < WIDTH; i++)
		{
			if (field[j][i] == 1)
			{
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else
				printw(".");
		}
	}
}

void PrintScore(int score)
{
	move(17, WIDTH + 11);
	printw("%8d", score);
}

void DrawNextBlock(int *nextBlock)
{
	int i, j;
	for (i = 0; i < 4; i++) // 첫 번째 next block 그리기
	{
		move(4 + i, WIDTH + 13);
		for (j = 0; j < 4; j++)
		{
			if (block[nextBlock[1]][0][i][j] == 1)
			{
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else
				printw(" ");
		}
	}

	for (i = 0; i < 4; i++) // 두 번째 next block 그리기
	{
		move(10 + i, WIDTH + 13);
		for (j = 0; j < 4; j++)
		{
			if (block[nextBlock[2]][0][i][j] == 1)
			{
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else
				printw(" ");
		}
	}
}

void DrawBlock(int y, int x, int blockID, int blockRotate, char tile)
{
	int i, j;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
		{
			if (block[blockID][blockRotate][i][j] == 1 && i + y >= 0)
			{
				move(i + y + 1, j + x + 1);
				attron(A_REVERSE);
				printw("%c", tile);
				attroff(A_REVERSE);
			}
		}

	move(HEIGHT, WIDTH + 10);
}

void DrawBox(int y, int x, int height, int width)
{
	int i, j;
	move(y, x);
	addch(ACS_ULCORNER);
	for (i = 0; i < width; i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	for (j = 0; j < height; j++)
	{
		move(y + j + 1, x);
		addch(ACS_VLINE);
		move(y + j + 1, x + width + 1);
		addch(ACS_VLINE);
	}
	move(y + j + 1, x);
	addch(ACS_LLCORNER);
	for (i = 0; i < width; i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
}

void play()
{
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM, &act, &oact);
	InitTetris();
	do
	{
		if (timed_out == 0)
		{
			alarm(1);
			timed_out = 1;
		}

		command = GetCommand();
		if (ProcessCommand(command) == QUIT)
		{
			alarm(0);
			DrawBox(HEIGHT / 2 - 1, WIDTH / 2 - 5, 1, 10);
			move(HEIGHT / 2, WIDTH / 2 - 4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}
	} while (!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT / 2 - 1, WIDTH / 2 - 5, 1, 10);
	move(HEIGHT / 2, WIDTH / 2 - 4);
	printw("GameOver!!");
	refresh();
	getch();
	newRank(score);
}

char menu()
{
	printw("1. play\n");
	printw("2. rank\n");
	printw("3. recommended play\n");
	printw("4. exit\n");
	return wgetch(stdscr);
}

/////////////////////////첫주차 실습에서 구현해야 할 함수/////////////////////////

int CheckToMove(char f[HEIGHT][WIDTH], int currentBlock, int blockRotate, int blockY, int blockX)
{
	// user code
	int i, j;
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if (block[currentBlock][blockRotate][i][j] == 1)
			{
				int moved_y = blockY + i, moved_x = blockX + j;

				if (f[moved_y][moved_x] == 1) // 블록을 놓으려고 하는 필드에 이미 블록이 쌓여져 있는 경우
					return 0;
				if (moved_y >= HEIGHT) // 블록의 실제 필드상의 y 좌표가 HEIGHT보다 큰 경우
					return 0;
				if (moved_x < 0 || moved_x >= WIDTH) // 블록의 실제 필드상의 x 좌표가 0보다 작거나 WIDTH보다 큰 경우
					return 0;
			}
		}
	}
	return 1;
}

void DrawChange(char f[HEIGHT][WIDTH], int command, int currentBlock, int blockRotate, int blockY, int blockX)
{
	// user code
	//1. 이전 블록 정보를 찾는다. ProcessCommand의 switch문을 참조할 것
	//2. 이전 블록 정보를 지운다. DrawBlock함수 참조할 것.
	//3. 새로운 블록 정보를 그린다.

	int prev_rotate = blockRotate, prev_y = blockY, prev_x = blockX;
	int i, j;
	int prev_shadow_y;

	switch (command) // 이전 블록의 정보 찾기 및 prev 변수 업데이트
	{
	case KEY_UP:
		prev_rotate = (blockRotate + 3) % 4;
		break;
	case KEY_DOWN:
		prev_y = blockY - 1;
		break;
	case KEY_RIGHT:
		prev_x = blockX - 1;
		break;
	case KEY_LEFT:
		prev_x = blockX + 1;
		break;
	}
	prev_shadow_y = getShadowY(prev_y, prev_x, currentBlock, prev_rotate); // 이전 그림자 위치 찾기

	for (i = 0; i < 4; i++) // 이전 블록과 그림자 정보 지우기
	{
		for (j = 0; j < 4; j++)
		{
			if (block[currentBlock][prev_rotate][i][j] == 1)
			{
				if (i + prev_y >= 0) // 이전 블록 지우기
				{
					move(i + prev_y + 1, j + prev_x + 1);
					printw(".");
				}
				if (i + prev_shadow_y >= 0) // 이전 그림자 지우기
				{
					move(i + prev_shadow_y + 1, j + prev_x + 1);
					printw(".");
				}
			}
		}

		DrawBlockWithFeatures(blockY, blockX, currentBlock, blockRotate); // 새로운 블록과 그림자 정보 그리기
	}
}

void BlockDown(int sig)
{
	// user code
	//강의자료 p26-27의 플로우차트를 참고한다.
	if (CheckToMove(field, nextBlock[0], blockRotate, blockY + 1, blockX))
	{ // 블록이 아래로 내려갈 수 있다면 ,blockY++ 후 필드에 그려주기
		blockY++;
		DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, blockY, blockX);
	}
	else
	{ // 블록이 아래로 내려갈 수 없는 경우
		if (blockY == -1)
			gameOver = 1;

		score += AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);
		score += DeleteLine(field);

		// nextBlock 재설정
		nextBlock[0] = nextBlock[1];
		nextBlock[1] = nextBlock[2];
		nextBlock[2] = rand() % 7;

		// Block 위치 초기화
		blockY = -1;
		blockX = WIDTH / 2 - 2;
		blockRotate = 0;

		DrawNextBlock(nextBlock);
		PrintScore(score);

		DrawField();
	}

	timed_out = 0; // timer reset
}

int AddBlockToField(char f[HEIGHT][WIDTH], int currentBlock, int blockRotate, int blockY, int blockX)
{
	// user code

	//Block이 추가된 영역의 필드값을 바꾼다.

	int i, j;
	int touched = 0;

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if (block[currentBlock][blockRotate][i][j] == 1)
			{
				f[blockY + i][blockX + j] = 1;

				// block이 필드에 추가되는 위치 바로 아래에 필드가 채워져 있거나
				// HEIGHT에 닿았다면, touched를 증가시킨다.
				if ((f[blockY + i + 1][blockX + j] == 1) | (blockY + i + 1 == HEIGHT))
					touched++;
			}
		}
	}
	return touched * 10;
}

int DeleteLine(char f[HEIGHT][WIDTH])
{
	// user code

	//1. 필드를 탐색하여, 꽉 찬 구간이 있는지 탐색한다.
	//2. 꽉 찬 구간이 있으면 해당 구간을 지운다. 즉, 해당 구간으로 필드값을 한칸씩 내린다.
	int i, j, k;
	int full_line = 0;
	int count = 0; // 완전히 채워진 라인 count

	for (i = 0; i < HEIGHT; i++)
	{
		for (j = 0; j < WIDTH; j++)
		{
			if (f[i][j] == 0)
			{
				full_line = 0;
				break;
			}
			full_line = 1;
		}

		if (full_line) // 완전히 채워진 라인이 존재하면 count++ 및 한 줄씩 내리기
		{
			count++;
			for (k = i - 1; k >= 0; k--) // 완전히 채워진 라인 위부터
			{
				for (j = 0; j < WIDTH; j++)
				{
					f[k + 1][j] = f[k][j];
				}
			}
		}
	}
	return count * count * 100;
}

///////////////////////////////////////////////////////////////////////////

void DrawShadow(int y, int x, int blockID, int blockRotate)
{
	// user code
	int shadow_y = getShadowY(y, x, blockID, blockRotate); // 그림자 위치를 찾는다.
	DrawBlock(shadow_y, x, blockID, blockRotate, '/');	   // 그림자를 그린다.
}

int getShadowY(int y, int x, int blockID, int blockRotate)
{
	int shadow_y;

	while (1) // 더 이상 내려갈 수 없는 위치를 찾는다.
	{
		y++;
		if (CheckToMove(field, blockID, blockRotate, y, x) == 0)
			break;
	}
	shadow_y = y - 1; // shadow의 y 위치

	return shadow_y;
}

void DrawBlockWithFeatures(int y, int x, int blockID, int blockRotate)
{
	DrawBlock(y, x, blockID, blockRotate, ' '); // 블록을 그린다.
	DrawShadow(y, x, blockID, blockRotate);		// 그림자를 그린다.
}

void createRankList()
{
	// user code
	FILE *fp;
	char name[NAMELEN];
	int score;
	int i;
	Node *curr;
	Node *temp;

	fp = fopen("rank.txt", "r");
	if (fp == NULL)
	{
		// 만약 rank.txt 파일이 존재하지 않는다면 새롭게 생성
		fp = fopen("rank.txt", "w");
		fclose(fp);
		return;
	}
	// 랭킹 정보를 읽어서 랭킹 목록 생성(linked list로 구현)
	fscanf(fp, "%d\n", &rank_num);

	for (i = 0; i < rank_num; i++)
	{
		fscanf(fp, "%s %d\n", name, &score);

		temp = (Node *)malloc(sizeof(Node));
		strcpy(temp->name, name);
		temp->score = score;
		temp->next = NULL;

		if (head == NULL)
		{
			head = temp;
			curr = head;
		}
		else
		{
			curr->next = temp;
			curr = curr->next;
		}
	}

	fclose(fp);
}

void rank()
{
	// user code
	int x, y, count;
	char search_name[NAMELEN];
	int search_rank;
	Node *temp;

	clear();
	printw("1. list ranks from X to Y\n");
	printw("2. list ranks by a specific name\n");
	printw("3. delete a specific rank\n");

	switch (wgetch(stdscr))
	{
	case '1':
		echo();
		printw("X: ");
		scanw("%d", &x);
		printw("Y: ");
		scanw("%d", &y);
		noecho();
		printw("       name       |   score   \n");
		printw("------------------------------\n");
		traverseList(x, y);
		break;
	case '2':
		echo();
		printw("input the name: ");
		scanw("%s", search_name);
		noecho();

		printw("       name       |   score   \n");
		printw("------------------------------\n");

		temp = head;
		count = 0;
		while (temp != NULL)
		{
			if (strcmp(temp->name, search_name) == 0)
			{
				printw(" %-16s | %d\n", temp->name, temp->score);
				count++;
			}
			temp = temp->next;
		}
		if (count == 0)
		{
			printw("\nsearch failure: no name in the list\n");
		}
		break;
	case '3':
		echo();
		printw("input the rank: ");
		scanw("%d", &search_rank);
		noecho();
		if (search_rank < 1 || search_rank > rank_num || head == NULL)
		{
			printw("\nsearch failure: the rank not in the list\n");
			break;
		}
		else
		{
			temp = head;
			count = 1;

			if (search_rank == 1)
			{
				head = temp->next;
				free(temp);
			}
			else
			{
				while (count < search_rank - 1)
				{
					temp = temp->next;
					count++;
				}
				temp->next = temp->next->next;
				free(temp->next);
			}
			printw("\nresult: the rank deleted\n");
			rank_num--;
		}
		break;
	default:
		break;
	}
	getch();
}

void traverseList(int x, int y)
{
	Node *temp;
	int range, count;

	// X, Y값 조정
	if (x == 0)
		x = 1;
	if (y == 0)
		y = rank_num;

	if (rank_num == 0)
		return;

	if (x < 0 || y < 0)
	{
		printw("\nsearch failure: no rank in the list\n");
		return;
	}
	range = y - x + 1;
	if (range <= 0)
	{
		printw("\nsearch failure: no rank in the list\n");
		return;
	}

	count = 1;
	temp = head;
	while (count < x)
	{
		temp = temp->next;
		count++;
	}

	for (count = 0; count < range; count++)
	{
		printw(" %-16s | %d\n", temp->name, temp->score);
		if (temp->next != NULL)
			temp = temp->next;
	}
}
void writeRankFile()
{
	// user code
	FILE *fp = fopen("rank.txt", "w");
	Node *curr;

	fprintf(fp, "%d\n", rank_num);
	while (head != NULL)
	{
		curr = head;
		fprintf(fp, "%s %d\n", curr->name, curr->score);
		head = head->next;
		free(curr);
	}
	fclose(fp);
}

void newRank(int score)
{
	// user code
	Node *new = (Node *)malloc(sizeof(Node));
	Node *curr;

	clear();
	// 사용자 정보 입력
	printw("your name: ");
	echo();
	getstr(new->name);
	noecho();
	new->score = score;
	new->next = NULL;

	// 랭킹 목록에 등록
	if (head == NULL)
	{
		head = new;
		rank_num++;
		return;
	}
	else
	{
		curr = head;
		if (rank_num == 1)
		{
			if (curr->score <= new->score)
			{
				new->next = curr;
				head = new;
			}
			else if (curr->score > new->score)
				curr->next = new;
			rank_num++;
			return;
		}
		else
		{
			while (curr->next != NULL)
			{
				if (curr->score <= new->score)
				{
					new->next = curr;
					head = new;
					break;
				}
				else if (curr->score > new->score && curr->next->score <= new->score)
				{
					new->next = curr->next;
					curr->next = new;
					break;
				}
				curr = curr->next;
			}
			if (curr->score > new->score)
				curr->next = new;
			rank_num++;
			return;
		}
	}
}

void DrawRecommend(int y, int x, int blockID, int blockRotate)
{
	// user code
}

int recommend(RecNode *root)
{
	int max = 0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수

	// user code

	return max;
}

void recommendedPlay()
{
	// user code
}
