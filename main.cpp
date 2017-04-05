#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#include "Screen.h"


typedef enum _GAME_STATE { INIT, READY, RUNNING, SUCCESS, FAILED, STOP, RESULT, OLLCLEAR } GAME_STATE;
typedef enum _DUDAGI_STATE { SETUP, UP, DOWN } DUDAGI_STATE;

typedef struct _STAGE_INFO
{
	int nCatchDudagi;
	clock_t LimitTime;
	clock_t UpLimitTime;
	clock_t DownLimitTime;
} STAGE_INFO;

typedef struct _MANGCHI
{
	int nIsAttack; //망치 상태
	clock_t StartTime; //타격 상태가 시작된 시각
	clock_t DelayTime; //타격 상태의 지연 시각
	int nIndex; // 좌표 배열에 접근하기 위한 인덱스
	int nStayX, nStayY; //대기 상태일 때 좌표
} MANGCHI;

typedef struct _DUDAGI
{
	DUDAGI_STATE nState; //두더지 상태
	clock_t StayTime;    //들어갔을 때 대기 시간]
	clock_t OutPutTime;
	clock_t OldTime;
} DUDAGI;

typedef struct _XY
{
	int nX, nY;
} XY;

XY g_Point[4] = {{2,10},{14,10},{27,10}, {40,10}};


STAGE_INFO g_StageInfo[10] = { {1, 1000*3 ,500, 500},{1, 1000*3 ,500, 500},{1, 1000*3 ,500, 500},{1, 1000*3 ,500, 500},{1, 1000*3 ,500, 500},{1, 1000*3 ,500, 500},{1, 1000*3 ,500, 500},{1, 1000*3 ,500, 500},{1, 1000*3 ,500, 500},{1, 1000*3 ,500, 500}};


GAME_STATE g_nGameState = INIT;
int	       g_nIsSuccess = 0;

DUDAGI g_Dudagi[9];
MANGCHI g_Mangchi;

int		g_nStage = -1;
int		g_nGrade = 0;
int		g_nStageCount = 1;
int		g_nDudagiCount = 0;
clock_t g_GameStartTime, g_UpdateOldTime, g_RemainTime;
int		g_nTotalGrade;

	
		

//FMOD_SYSTEM *g_System;
//FMOD_SOUND *g_Sound[7];
//FMOD_CHANNEL *g_Channel[7];
char *g_strSoundFile[7] = { "배경음.mp3", "미션실패.mp3", "레디.mp3", "러닝.mp3", "죽음.mp3", "효과음.mp3", "성공.wav"};

void Mangchi(int x, int y)
{
	ScreenPrint(x, y-1, "☆");
	ScreenPrint(x, y,   "");
	ScreenPrint(x, y+1, "");
}
void MangchiReady(int x, int y)
{
	ScreenPrint(x, y-2, "");
	ScreenPrint(x, y-1 ,"");
	ScreenPrint(x, y,	"");
	ScreenPrint(x,y+1,  "");
	ScreenPrint(x,y+2,  "");
	ScreenPrint(x,y+3,  "");
	ScreenPrint(x,y+4,  "");
	ScreenPrint(x,y+5,  "");

}
void InitScreen()
{
	ScreenPrint( 0,0, "┌────────────────────────┐");
	ScreenPrint( 0,1, "│                                                │");
	ScreenPrint( 0,2, "│                                                │");
	ScreenPrint( 0,3, "│                                                │");
	ScreenPrint( 0,4, "│                                                │");
	ScreenPrint( 0,5, "│                                                │");
	ScreenPrint( 0,6, "│                                                │");
	ScreenPrint( 0,7, "│                                                │");
	ScreenPrint( 0,8, "│                                                │");
	ScreenPrint( 0,9, "│                                                │");
	ScreenPrint( 0,10,"│                                                │");
	ScreenPrint( 0,11,"│                                                │");
	ScreenPrint( 0,12,"│                                                │");
	ScreenPrint( 0,13,"│                                                │");
	ScreenPrint( 0,14,"│                                                │");
	ScreenPrint( 0,15,"│                                                │");
	ScreenPrint( 0,16,"│                                                │");
	ScreenPrint( 0,17,"│                                                │");
	ScreenPrint( 0,18,"│                                                │");
	ScreenPrint( 0,19,"│                    [Space] 누르시오.           │");
	ScreenPrint( 0,20,"│                                                │");
	ScreenPrint( 0,21,"│                                                │");
	ScreenPrint( 0,22,"│                                                │");
	ScreenPrint( 0,23,"└────────────────────────┘");
}

void ReadyScreen()
{
	ScreenPrint( 0,0, "┌────────────────────────┐");
	ScreenPrint( 0,1, "│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,2, "│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,3, "│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,4, "│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,5, "│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,6, "│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,7, "│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,8, "│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,9, "│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,10,"│■■■■■                            ■■■■■│");
	ScreenPrint( 0,11,"│■■■■■          스테이지          ■■■■■│");
	ScreenPrint( 0,12,"│■■■■■                            ■■■■■│");
	ScreenPrint( 0,13,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,14,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,15,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,16,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,17,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,18,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,19,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,20,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,21,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,22,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,23,"└────────────────────────┘");
}

void SuccessScreen()
{
   	ScreenPrint( 0,0, "┌────────────────────────┐");
	ScreenPrint( 0,1, "│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,2, "│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,3, "│■■■■■                            ■■■■■│");
	ScreenPrint( 0,4, "│■■■■■     [ ] 스테이지 성공!     ■■■■■│");
	ScreenPrint( 0,5, "│■■■■■                            ■■■■■│");
	ScreenPrint( 0,6, "│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,7, "│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,8, "│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,9, "│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,10,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,11,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,12,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,13,"│■■■■■                            ■■■■■│");
	ScreenPrint( 0,14,"│■■■■■    잡은 두더지 수[    ]    ■■■■■│");
	ScreenPrint( 0,15,"│■■■■■                            ■■■■■│");
	ScreenPrint( 0,16,"│■■■■■     점수[     ]            ■■■■■│");
	ScreenPrint( 0,17,"│■■■■■                            ■■■■■│");
	ScreenPrint( 0,18,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,19,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,20,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,21,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,22,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,23,"└────────────────────────┘");
}

void RunningScreen()
{
	ScreenPrint( 0,0, "┌────────────────────────┐");
	ScreenPrint( 0,1, "│                                                │");
	ScreenPrint( 0,2, "│                                                │");
	ScreenPrint( 0,3, "│                                                │");
	ScreenPrint( 0,4, "│                                                │");
	ScreenPrint( 0,5, "│                                                │");
	ScreenPrint( 0,6, "│                                                │");
	ScreenPrint( 0,7, "│                                                │");
	ScreenPrint( 0,8, "│                                                │");
	ScreenPrint( 0,9, "│                                                │");
	ScreenPrint( 0,10,"│                                                │");
	ScreenPrint( 0,11,"│                                                │");
	ScreenPrint( 0,12,"│                                                │");
	ScreenPrint( 0,13,"│                                                │");
	ScreenPrint( 0,14,"│                                                │");
	ScreenPrint( 0,15,"│                                                │");
	ScreenPrint( 0,16,"│                                                │");
	ScreenPrint( 0,17,"│                                                │");
	ScreenPrint( 0,18,"│                                                │");
	ScreenPrint( 0,19,"│                                                │");
	ScreenPrint( 0,20,"│                                                │");
	ScreenPrint( 0,21,"│                                                │");
	ScreenPrint( 0,22,"│                                                │");
	ScreenPrint( 0,23,"└────────────────────────┘");
}

void FailureScreen()
{
	ScreenPrint( 0,0, "┌────────────────────────┐");
	ScreenPrint( 0,1, "│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,2, "│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,3, "│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,4, "│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,5, "│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,6, "│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,7, "│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,8, "│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,9, "│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,10,"│■■■■■                            ■■■■■│");
	ScreenPrint( 0,11,"│■■■■■          미션실패          ■■■■■│");
	ScreenPrint( 0,12,"│■■■■■ 재시도: [Y] 그만하기: [N]  ■■■■■│");
	ScreenPrint( 0,13,"│■■■■■                            ■■■■■│");
	ScreenPrint( 0,14,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,15,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,16,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,17,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,18,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,19,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,20,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,21,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,22,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,23,"└────────────────────────┘");
}
 // NOte: 실패 했을 경우에 출력되면서 다시 할것인지를 묻는 화면이다.
void ResultScreen()
{
	ScreenPrint( 0,0, "┌────────────────────────┐");
	ScreenPrint( 0,1, "│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,2, "│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,3, "│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,4, "│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,5, "│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,6, "│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,7, "│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,8, "│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,9, "│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,10,"│■■■■■                            ■■■■■│");
	ScreenPrint( 0,11,"│■■■■■       총 점수 [     ]      ■■■■■│");
	ScreenPrint( 0,12,"│■■■■■                            ■■■■■│");
	ScreenPrint( 0,13,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,14,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,15,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,16,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,17,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,18,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,19,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,20,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,21,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,22,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,23,"└────────────────────────┘");
}

void OllClearScreen()
{   
	ScreenPrint( 0,0, "┌────────────────────────┐");
	ScreenPrint( 0,1, "│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,2, "│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,3, "│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,4, "│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,5, "│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,6, "│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,7, "│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,8, "│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,9, "│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,10,"│■■■■■            모든            ■■■■■│");
	ScreenPrint( 0,11,"│■■■■■          스테이지          ■■■■■│");
	ScreenPrint( 0,12,"│■■■■■           클리어!!         ■■■■■│");
	ScreenPrint( 0,13,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,14,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,15,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,16,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,17,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,18,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,19,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,20,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,21,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,22,"│■■■■■■■■■■■■■■■■■■■■■■■■│");
	ScreenPrint( 0,23,"└────────────────────────┘");
}

void SoundInit()
{
//	int i;
//
////	FMOD_System_Create(&g_System);
////	FMOD_System_Init(g_System, 32, FMOD_INIT_NORMAL,NULL);
//
//	for(i=0 ; i<2; i++)
//		FMOD_System_CreateSound(g_System, g_strSoundFile[i],FMOD_LOOP_NORMAL, 0 ,&g_Sound[i]);
//	for ( i = 2; i < 7; i++)
//		FMOD_System_CreateSound(g_System, g_strSoundFile[i],FMOD_HARDWARE, 0 , &g_Sound[i]);
}


void Init()
{
	int i;

	if( g_nStage == -1 ) // 1번만 초기화가 되는 부분에 대한 처리 사항
	{
//		 SoundInit();
		 g_nStage = 0;
//		 FMOD_System_PlaySound( g_System, FMOD_CHANNEL_FREE, g_Sound[0], 0, &g_Channel[0] );
	}

	g_nDudagiCount = 0; // 잡은 두더지 개수
	g_nGrade = 0;

	// Note : 망치 셋업
	g_Mangchi.nIsAttack = 0;
	g_Mangchi.nIndex = 0;
	g_Mangchi.nStayX = 38;
	g_Mangchi.nStayY = 10;
	g_Mangchi.StartTime = g_GameStartTime;
	g_Mangchi.DelayTime = 200;

	for( i = 0 ; i < 9 ; i++ )
	{
		g_Dudagi[i].StayTime = rand() % g_StageInfo[g_nStage].DownLimitTime + 100; 
		g_Dudagi[i].OldTime = clock();
		g_Dudagi[i].nState = SETUP;
		g_Dudagi[i].OutPutTime = rand() % g_StageInfo[g_nStage].UpLimitTime + 50;
	}
}

void Update()
{
	int i;
	clock_t CurTime = clock();

	switch( g_nGameState )
	{
	case READY :
				if( CurTime - g_UpdateOldTime > 500 ) // 2초
				{
					g_nGameState = RUNNING;
					g_GameStartTime = CurTime;
				}
				break;
	case RUNNING :
				if( (CurTime - g_GameStartTime) > g_StageInfo[g_nStage].LimitTime )
				{
					g_nGameState = STOP;
					return ;
				}else{
					//두더지 업데이트
					for( i = 0 ; i < 4 ; i++ )
					{
						switch( g_Dudagi[i].nState )
						{
						case SETUP :
									g_Dudagi[i].OldTime = CurTime;
									g_Dudagi[i].OutPutTime = rand() % (int)g_StageInfo[g_nStage].UpLimitTime + 700;
									g_Dudagi[i].StayTime = rand() % (int)g_StageInfo[g_nStage].DownLimitTime + 1000;
									g_Dudagi[i].nState = UP;
									break;
						case UP :
									if( CurTime - g_Dudagi[i].OldTime > g_Dudagi[i].OutPutTime )
									{
										g_Dudagi[i].OldTime = CurTime;
										g_Dudagi[i].nState = DOWN;
									}
									break;
						case DOWN :
									if( CurTime - g_Dudagi[i].OldTime > g_Dudagi[i].StayTime )
										g_Dudagi[i].nState = SETUP;
									break;
						}
					}
					
					// 망치 업데이트
					if( g_Mangchi.nIsAttack )
					{
						//충돌 테스트 망치는 오직 하나의 두더지만 잡을 수 있기에 하나만 잡았으면 충돌 체크를 빠져 나온다.
						for( i = 0; i < 4 ; i++)
						{
							if( g_Dudagi[i].nState == UP && i == g_Mangchi.nIndex )
							{
								g_nGrade += 10;
								g_nDudagiCount++;
								g_Dudagi[i].nState = DOWN; //죽었으면 다운 상태로 바로 전환을 하도록 한다.
//								FMOD_System_PlaySound( g_System, FMOD_CHANNEL_FREE, g_Sound[4], 0, &g_Channel[4] );
								break;
							}
						}
					    
						// Note : 망치의 상태를 변경해주는 부분 망치로 쳤을 때에 일정한 시간 동안 머물도록 하기 위한 부분
						if( CurTime - g_Mangchi.StartTime > g_Mangchi.DelayTime )
						{
							g_Mangchi.nIsAttack = 0;
						}
					}

					g_RemainTime = ( g_StageInfo[g_nStage].LimitTime - ( CurTime - g_GameStartTime) ) / 300 ; //게임 진행 남은 시간
				}
				break;
	case STOP :
				// 성공이냐 실패를 판단해주어서 출력을 해주는 부분이 와야 한다.
				if( g_nDudagiCount >= g_StageInfo[g_nStage].nCatchDudagi )
				{
					g_UpdateOldTime = CurTime;
					g_nGameState = SUCCESS;
					g_nTotalGrade += g_nGrade;
//					FMOD_System_PlaySound( g_System, FMOD_CHANNEL_FREE, g_Sound[6], 0, &g_Channel[6] ) ; // 미션 성공 사운드

				}else{
					g_nGameState = FAILED;
//					FMOD_System_PlaySound( g_System, FMOD_CHANNEL_FREE, g_Sound[1], 0, &g_Channel[1] ) ; // 미션 실패 사운드
				}
				break;

		        
	case SUCCESS :
				if( CurTime - g_UpdateOldTime > 1000 )
				{

					g_UpdateOldTime = CurTime;
					g_nGrade = 0;
					++g_nStage;
					Init();
					if( g_nStage > 9)
					{
						g_nGameState = OLLCLEAR;
					}else{
					g_nGameState = READY;
					}
				}
				break;
	 		
	}
}

void Render()
{
	int i;
	char string[100];

	ScreenClear();

	switch( g_nGameState )
	{
	case INIT :
				if( g_nStage == 0 )
					InitScreen();
				break;

	case READY :
				ReadyScreen();
				sprintf( string, "%d" , g_nStage + 1);
				ScreenPrint( 18,11, string);
				break;


	case RUNNING:
				RunningScreen();

				sprintf( string, "[목표 두더지] : %d [잡은 두더지] : %d", g_StageInfo[g_nStage].nCatchDudagi, g_nDudagiCount );
				ScreenPrint( 2, 1, string);
				sprintf( string, "[스테이지] : %d [점수] : %d [남은 시간] : %d ", g_nStage + 1, g_nGrade, g_RemainTime );
				ScreenPrint( 2, 2, string);
				
				

				for( i = 0 ; i < 4 ; i++ )
				{
					if( g_Dudagi[i].nState == UP ){
						
						ScreenPrint( g_Point[i].nX, g_Point[i].nY,	"■■■■" );
						
					}
					ScreenPrint( g_Point[i].nX, g_Point[i].nY + 1, "" );
				}
				
				if( g_Mangchi.nIsAttack )
					Mangchi( g_Point[g_Mangchi.nIndex].nX, g_Point[g_Mangchi.nIndex].nY );
				else
					MangchiReady( g_Mangchi.nStayX, g_Mangchi.nStayY );
				break;

	case SUCCESS:
				SuccessScreen();
				sprintf( string, "%d", g_nStage + 1 );
				ScreenPrint( 18, 4, string );
				sprintf( string, "%d", g_nDudagiCount );
				ScreenPrint( 32, 14, string );
				sprintf( string, "%d", g_nTotalGrade );
				ScreenPrint( 23, 16, string );
				break;

	case FAILED:
				FailureScreen();
				break;

	case RESULT:
				ResultScreen();
				sprintf( string, "%d", g_nTotalGrade );
				ScreenPrint( 28, 11, string );
				break;
	case OLLCLEAR:
				ResultScreen();
				sprintf( string, "%d", g_nTotalGrade );
				ScreenPrint( 28, 11, string );
				break;

	}
	ScreenFlipping();
}

void Release()
{
	int i;
	//for ( i = 0 ; i < 7 ; i++)
	//	FMOD_Sound_Release( g_Sound[i] );

	//FMOD_System_Close( g_System );
	//FMOD_System_Release( g_System );
}

int main(void)
{
	int nKey;

	ScreenInit();
	Init();  //초기화

	while( 1 )
	{
		if( _kbhit() )
		{
			if( g_nGameState ==RESULT )
					break;

			nKey = _getch();
			switch( nKey )
			{
			case ' ' :
						if( g_nGameState == INIT && g_nStage == 0 )
						{
							g_nGameState = READY;
				//			FMOD_Channel_Stop( g_Channel[0] ); //메인음 중지
				//			FMOD_System_PlaySound( g_System, FMOD_CHANNEL_FREE, g_Sound[2], 0, &g_Channel[2]); //READY 사운드 출력
							g_UpdateOldTime = clock(); // ready를 일정시간 지속해 주기 위해
							
						}
						break;
			case '1' : // 망치 키 입력 1~9  까지
			case '2' :
			case '3' :
			case '4' :
			case '5' :
			case '6' :
			case '7' :
			case '8' :
			case '9' :
						if( g_Mangchi.nIsAttack == 0 && g_nGameState == RUNNING )
						{
			//				FMOD_Channel_Stop( g_Channel[2] );
							g_Mangchi.nIndex = nKey - '1';
							g_Mangchi.StartTime = clock();
							g_Mangchi.nIsAttack = 1;
				//			FMOD_System_PlaySound( g_System, FMOD_CHANNEL_FREE, g_Sound[5], 0, &g_Channel[5] );
						}
						break;
			case 'y' :
			case 'Y' :
						if( g_nGameState == FAILED )
						{
							Init();
							g_nGameState = READY;
						//	FMOD_Channel_Stop( g_Channel[1] ); // 미션 실패 사운드 출력 중지;
					//		FMOD_System_PlaySound( g_System, FMOD_CHANNEL_FREE, g_Sound[2], 0, &g_Channel[2]); // READY 사운드 출력
							g_nGrade = 0;
							g_UpdateOldTime = clock();
						}
						break;
			case 'n' :
			case 'N' :
						if( g_nGameState == FAILED )
						{
							g_nGameState = RESULT;
				//			FMOD_Channel_Stop( g_Channel[1] ); //미션 실패 사운드 출력 중지
						}
						break;
			}
		}

		Update(); // 데이터 갱신
		Render(); // 화면 출력
	//	FMOD_System_Update( g_System );
	}

	Release(); //해제
	ScreenRelease();

	return 0;
}















					