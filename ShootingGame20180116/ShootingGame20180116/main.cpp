#include "DxLib.h"
#include<stdlib.h>
#include<time.h>

#define PLAYERMAX_SHOT 4//�v���C���[���łĂ�V���b�g��
#define MAX_Apple 3 //�����S�̐�
#define PLAYER_HP 3 //�v���C���[��HP

struct SIZE2D {
	int width;
	int height;
};

struct POINT2DF { // ���W��\��
	float x;
	float y;
};
struct VECTOR2DF { // �ړ��ʂ�\��
	float x;
	float y;
};

struct CHARADATA {
	struct VECTOR2DF move;
	struct POINT2DF pos;
	int r;
	int color;
	int image;
	int imageJump;
	struct SIZE2D imageSize;
	BOOL isLeft;
	BOOL onGround;
	BOOL life; //1�Ȃ琶���A0�Ȃ����
};
struct SHOTDATA {
	struct VECTOR2DF move;
	struct POINT2DF pos;
	int r;
	int color;
	BOOL life; //1�Ȃ琶���A0�Ȃ����
};

//  �Q�[�����
enum {
	GAME_TITLE,    //�^�C�g��
	GAME_MAIN,     //���C��
	GAME_ENDING,   //�G���f�B���O
	GAME_BADEND,   //�Q�[���I�[�o�[
	GAME_END,      //�I��
};

int i = 0, s, s1, s2, check = 0, Ehit[18], Phit[9], media[9], TextColer, sound[6];
int deg = 0, dDeg = 5, r = 100;
int keyZNow, keyZOld, Ehitcount;
int EcountMax=25; //���C�|������N���A�� 
int function_status = GAME_TITLE;
char keyBuf[256], message[100];
float rad;
const struct SIZE2D screenSize = { 640, 480 };
const float dt = (float)1 / 60;
struct VECTOR2DF vec, moveVec;
struct POINT2DF pos[2];
struct CHARADATA shot[PLAYERMAX_SHOT][PLAYERMAX_SHOT]; //�v���C���[shot
struct CHARADATA player;
struct CHARADATA enemy[3];
struct CHARADATA object[3];
const struct VECTOR2DF jumpVec = { 0, -700 };
const struct VECTOR2DF gravVec = { 0, 30 };
struct VECTOR2DF inerVec = { 0, 0 };
BOOL PhitFlag; //�v���C���[�̑ł����e������������
BOOL Phitcount = MAX_Apple;


int Title() {


	StopSoundMem(sound[1]);

	//�����A�`��
	DrawExtendGraph(-40, -40, 1400, 550, media[3], TRUE);//�w�i
	SetFontSize(60);
	DrawFormatString(screenSize.width / 10 - 50, screenSize.height / 8 + 30, TextColer, " ����������%d�C�������Ƃ�, pple�����I",EcountMax);
	DrawString(screenSize.width / 10 - 50, screenSize.height / 8 + 30, " �@�@�@�@�@�@�@�@�@�@�@�@�@ A", GetColor(255, 0, 0));
	SetFontSize(50);
	DrawString(screenSize.width / 2, screenSize.height / 2 - 30, "�X�y�[�X�L�[�F�X�^�[�g\n\n", TextColer);
	SetFontSize(30);
	DrawString(100, 300, "�G�X�P�[�v�F�I��\n\n", TextColer);
	DrawString(100, 350, "Z�L�[�F�e��ł�\n\n���F�W�����v", TextColer);

	Phitcount = MAX_Apple;
	Ehitcount = 0;

	for (i = 0; i < 3; i++)
	{
		enemy[i].life = 1;
		enemy[i].pos.x = 1280;
		enemy[i].move.x = 5;
	}

	//�v���C���[�̑���
	if (keyBuf[KEY_INPUT_SPACE] == 1)
		return GAME_MAIN;
	if (keyBuf[KEY_INPUT_ESCAPE] == 1)
		return GAME_END;
	if (keyBuf[KEY_INPUT_C] == 1)
		return GAME_ENDING;
	if (keyBuf[KEY_INPUT_V] == 1)
		return GAME_BADEND;
	return GAME_TITLE;
}

int GameMain() {

	StopSoundMem(sound[0]);

	if (keyBuf[KEY_INPUT_ESCAPE] == 1)
		return GAME_END;

	if (keyBuf[KEY_INPUT_A] == 1)
	{
		Phitcount = 100;
	}

#pragma region �f�o�b�N�p 

	//	if (keyBuf[KEY_INPUT_X] == 1)
	//	{
	//		Phitcount = MAX_FRUITS;
	//	}
	//if (EhitFlag || PhitFlag)
	//DrawFormatString(screenSize.width / 2, screenSize.height / 2 - 30, GetColor(255, 0, 0), "������");
	//else
	//DrawFormatString(screenSize.width / 2, screenSize.height / 2, TextColer, "�������ĂȂ�");

#pragma endregion

	if (player.life) //�v���C���[�������Ă�����
	{
		//�V���b�g
		if (i >= PLAYERMAX_SHOT)
			i = 0;
		if (keyBuf[KEY_INPUT_Z] == 1)
		{
			if (check == 0)
			{
				for (i = 0; i < PLAYERMAX_SHOT; i++)
				{
					if (!shot[i][i].life)
					{
						shot[i][i].life = 1;
						shot[i][i].pos.x = player.pos.x;
						shot[i][i].pos.y = player.pos.y;
						shot[i][i].move.x = 7;
						shot[i][i].r = 10;
						shot[i][i].color = GetColor(90, 90, 90);
						break;
					}
				}
				check = 1;
				PlaySoundMem(sound[4], DX_PLAYTYPE_BACK);
			}
		}
		else
			check = 0;

		if (keyBuf[KEY_INPUT_UP] == 1)
			keyZNow = 1;
		else
			keyZNow = 0;
		if (keyZNow == 1 && keyZOld == 0) {
			strcpy_s(message, 100, "Z��������"); // �����ɃW�����v�̏�������������΂悢
			inerVec = jumpVec;
			player.onGround = FALSE;
			if (keyBuf[KEY_INPUT_RIGHT] == 1)
				inerVec.x += moveVec.x;
			if (keyBuf[KEY_INPUT_LEFT] == 1)
				inerVec.x -= moveVec.x;
		}
		else
			keyZOld = keyZNow;
		if (!player.onGround) {
			inerVec.x += gravVec.x; inerVec.y += gravVec.y;
		}
		player.pos.x += inerVec.x * dt; player.pos.y += inerVec.y * dt;
		if (player.pos.y > 400) {
			player.pos.y = 400;
			player.onGround = TRUE;
			inerVec.x = 0;
			inerVec.y = 0;
		}

	}

	PhitFlag = 0;

	//�`��
	DrawExtendGraph(-40, -40, 1400, 550, media[3], TRUE);//�w�i
	SetFontSize(30);
	DrawFormatString(250, 0, TextColer, "�����S�c�萔�F%d��", Phitcount);
	DrawFormatString(250, 40, TextColer, "�|�������F%d�H", Ehitcount);
	DrawLine(200, 0, 200, 480, GetColor(255, 0, 0));

	//�v���C���[
	if (player.life)
	{
		DrawCircle(player.pos.x, player.pos.y, player.r, GetColor(255, 255, 255), TRUE);//�v���C���[
		DrawGraph(player.pos.x - 100, player.pos.y - 100, media[0], TRUE);
	}

	//�I�u�W�F�N�g   �ǂ��ɂ������ƊȒP�ɂł���
	if (Phitcount >= 3)
	{
		DrawGraph(object[0].pos.x, object[0].pos.y, media[4], TRUE); //�����S
		DrawGraph(object[1].pos.x, object[1].pos.y, media[4], TRUE); //�����S
		DrawGraph(object[2].pos.x, object[2].pos.y, media[4], TRUE); //�����S	
	}
	if (Phitcount == 2)
	{
		DrawGraph(object[1].pos.x, object[1].pos.y, media[4], TRUE); //�����S
		DrawGraph(object[2].pos.x, object[2].pos.y, media[4], TRUE); //�����S	
	}
	if (Phitcount == 1)
	{
		DrawGraph(object[2].pos.x, object[2].pos.y, media[4], TRUE); //�����S
	}
	if (Phitcount == 0)
	{
		PlaySoundMem(sound[3], DX_PLAYTYPE_BACK);
		return GAME_BADEND;
	}

	if (PhitFlag == 1)
	{
		Ehitcount++;

		if (Ehitcount == 50)
		{
			return GAME_ENDING;
		}
	}
#pragma region �G�Ƃ̃V���b�g�����蔻��
	
	for (i = 0; i < PLAYERMAX_SHOT; i++)
	{
		if (shot[i][i].life)
		{
			shot[i][i].pos.x += shot[i][i].move.x;

			if (1280 < shot[i][i].pos.x + shot[i][i].r)//�v���C���[�̒e��0�𒴂�����
				shot[i][i].life = 0;

			else//�v���C���[�̒e�`��
				DrawCircle(shot[i][i].pos.x, shot[i][i].pos.y, shot[i][i].r, shot[i][i].color, TRUE);


			if (enemy[0].life) //���̓G�̓����蔻��
			{
				Phit[0] = enemy[0].pos.x - shot[i][i].pos.x; Phit[1] = enemy[0].pos.y - shot[i][i].pos.y; Phit[2] = enemy[0].r + shot[i][i].r;

				if (Phit[0] * Phit[0] + Phit[1] * Phit[1] < Phit[2] * Phit[2])
				{
					PhitFlag = 1;
					enemy[0].life = 0;
					shot[i][i].life = 0;

					if (!enemy[0].life)
					{
						if (!enemy[1].life)
						{
							enemy[1].life = 1;
							enemy[1].pos.x = 1280;
							s1 = rand() % 5 + 1;
						}
						if (!enemy[2].life)
						{
							enemy[2].life = 1;
							enemy[2].pos.x = 1280;
							s2 = rand() % 5 + 1;
						}
					}
				}
			}

			if (enemy[1].life) //���̓G�̓����蔻��
			{
				Phit[3] = enemy[1].pos.x - shot[i][i].pos.x; Phit[4] = enemy[1].pos.y - shot[i][i].pos.y; Phit[5] = enemy[1].r + shot[i][i].r;
				if (Phit[3] * Phit[3] + Phit[4] * Phit[4] < Phit[5] * Phit[5])
				{
					PhitFlag = 1;
					enemy[1].life = 0;
					shot[i][i].life = 0;
					if (!enemy[1].life)
					{
						if (!enemy[0].life)
						{
							enemy[0].life = 1;
							enemy[0].pos.x = 1280;
							s = rand() % 5 + 1;
						}
						if (!enemy[2].life)
						{
							enemy[2].life = 1;
							enemy[2].pos.x = 1280;
							s2 = rand() % 5 + 1;
						}
					}
				}
			}

			if (enemy[2].life) //��̓G�̓����蔻��
			{
				Phit[6] = enemy[2].pos.x - shot[i][i].pos.x; Phit[7] = enemy[2].pos.y - shot[i][i].pos.y; Phit[8] = enemy[2].r + shot[i][i].r;

				if (Phit[6] * Phit[6] + Phit[7] * Phit[7] < Phit[8] * Phit[8])

				{
					PhitFlag = 1;
					enemy[2].life = 0;
					shot[i][i].life = 0;
					if (!enemy[2].life)
					{
						if (!enemy[1].life)
						{
							enemy[1].life = 1;
							enemy[1].pos.x = 1280;
							s1 = rand() % 5 + 1;
						}
						if (!enemy[0].life)
						{
							enemy[0].life = 1;
							enemy[0].pos.x = 1280;
							s = rand() % 5 + 1;
						}
					}
				}
			}
		}
	}
#pragma endregion

#pragma region �G�������Ă�����
	if (enemy[0].life)//���̓G�������Ă�����	
	{
		DrawCircle(enemy[0].pos.x - 30, enemy[0].pos.y, enemy[0].r, GetColor(255, 0, 0), FALSE);//�G1
		DrawGraph(enemy[0].pos.x - 170, enemy[0].pos.y - 125, media[1], TRUE);

		if (player.life)
		{
			if (enemy[0].life)
			{
				enemy[0].pos.x -= s + 5; //�ړ��X�s�[�h
			}
			if (100 > enemy[0].pos.x)
			{
				enemy[0].life = 0;
				Phitcount--;
				PlaySoundMem(sound[5], DX_PLAYTYPE_BACK);

				if (!enemy[0].life)
				{
					if (!enemy[1].life)
					{
						enemy[1].life = 1;
						enemy[1].pos.x = 1280;
						s1 = rand() % 5;
					}
					if (!enemy[2].life)
					{
						enemy[2].life = 1;
						enemy[2].pos.x = 1280;
						s2 = rand() % 5;
					}
				}
			}
		}
	}

	if (enemy[1].life)//���̓G�������Ă�����
	{
		DrawCircle(enemy[1].pos.x - 30, enemy[1].pos.y - 10, enemy[1].r, GetColor(0, 0, 255), FALSE);//�G2
		DrawGraph(enemy[1].pos.x - 170, enemy[1].pos.y - 150, media[2], TRUE);

		if (player.life)
		{
			if (enemy[1].life)
			{
				enemy[1].pos.x -= s1 + 3; //�ړ��X�s�[�h
			}
			if (100 > enemy[1].pos.x)
			{
				enemy[1].life = 0;
				Phitcount--;
				PlaySoundMem(sound[5], DX_PLAYTYPE_BACK);

				if (!enemy[1].life)
				{
					if (!enemy[0].life)
					{
						enemy[0].life = 1;
						enemy[0].pos.x = 1280;
						s = rand() % 5;
					}
					if (!enemy[2].life)
					{
						enemy[2].life = 1;
						enemy[2].pos.x = 1280;
						s2 = rand() % 5;
					}
				}
			}
		}
	}

	if (enemy[2].life)//��̓G�������Ă�����
	{
		DrawCircle(enemy[2].pos.x - 30, enemy[2].pos.y - 10, enemy[2].r, GetColor(0, 255, 0), FALSE);//�G3
		DrawGraph(enemy[2].pos.x - 165, enemy[2].pos.y - 150, media[2], TRUE);

		if (player.life)
		{
			if (enemy[2].life)

			{
				enemy[2].pos.x -= s2 + 4; //�ړ��X�s�[�h
			}
			if (100 > enemy[2].pos.x)
			{
				enemy[2].life = 0;
				Phitcount--;
				PlaySoundMem(sound[5], DX_PLAYTYPE_BACK);

				if (!enemy[2].life)
				{
					if (!enemy[1].life)
					{
						enemy[0].life = 1;
						enemy[0].pos.x = 1280;
						s = rand() % 5;
					}
					if (!enemy[2].life)
					{
						enemy[2].life = 1;
						enemy[2].pos.x = 1280;
						s2 = rand() % 5;
					}
				}
			}

		}
	}
#pragma endregion

	if (PhitFlag == 1) //�G�����C�|������
	{
		Ehitcount++;

		if (Ehitcount == EcountMax)
		{
			PlaySoundMem(sound[2], DX_PLAYTYPE_BACK);
			return GAME_ENDING;
		}
	}

	return GAME_MAIN;
}

int Ending() //�G���f�B���O
{
	StopSoundMem(sound[0]);
	StopSoundMem(sound[1]);
	DrawBox(0, 0, 1280, 480, GetColor(0, 60, 0), TRUE);
	DrawGraph(screenSize.width / 2 + 500, screenSize.height / 2 - 300, media[5], TRUE);
	DrawGraph(screenSize.width / 2 + 100, screenSize.height / 4, media[6], TRUE);
	shot[PLAYERMAX_SHOT][PLAYERMAX_SHOT].life = 0;
	SetFontSize(70);
	DrawFormatString(screenSize.width / 4, screenSize.height / 6, TextColer, "�N���A�I");
	SetFontSize(30);
	DrawFormatString(screenSize.width / 4, screenSize.height / 4 + 80, TextColer, "�G�X�P�[�v�L�[�ŏI��\nX�L�[�Ń^�C�g����");
	if (keyBuf[KEY_INPUT_ESCAPE] == 1)
		return GAME_END;
	if (keyBuf[KEY_INPUT_X] == 1)
		return GAME_TITLE;
	return GAME_ENDING;
}

int Badend() //�Q�[���I�[�o�[
{
	StopSoundMem(sound[0]);
	StopSoundMem(sound[1]);
	DrawBox(0, 0, 1280, 480, GetColor(0, 60, 0), TRUE);
	DrawGraph(screenSize.width / 2, screenSize.height / 2 - 200, media[7], TRUE);
	DrawGraph(screenSize.width / 2 + 500, screenSize.height / 2, media[8], TRUE);
	SetFontSize(70);
	DrawFormatString(screenSize.width / 6, screenSize.height / 6, TextColer, "�c�O�I");
	SetFontSize(30);
	DrawFormatString(screenSize.width / 6, screenSize.height / 4 + 80, TextColer, "�G�X�P�[�v�L�[�ŏI��\nX�L�[�Ń^�C�g����");
	if (keyBuf[KEY_INPUT_ESCAPE] == 1)
		return GAME_END;
	if (keyBuf[KEY_INPUT_X] == 1)
		return GAME_TITLE;
	return GAME_BADEND;
}

int WINAPI WinMain(HINSTANCE hI, HINSTANCE hP, LPSTR lpC, int nC)
{
	const float dt = (float)1 / 60;
	const struct VECTOR2DF moveVec = { 320, 240 };
	player.onGround = FALSE;
	ChangeWindowMode(TRUE);
	SetGraphMode(1280, 480, 32);
	if (DxLib_Init() == -1)
		return -1;
	SetDrawScreen(DX_SCREEN_BACK);
	srand((unsigned int)time(NULL));
	TextColer = GetColor(255, 255, 255);

	//�v���C���[���
	player.pos.x = 300;
	player.pos.y = 400;
	player.r = 1;
	player.life = 1;
	media[0] = LoadGraph(".\\media\\job_hunter.png");
	//���̓G���
	enemy[0].pos.x = 1280;
	enemy[0].pos.y = 400;
	enemy[0].r = 40;
	enemy[0].life = 1;
	enemy[0].color = GetColor(0, 60, 0);
	media[1] = LoadGraph(".\\media\\bluebird_baka.png");
	//���̓G���
	enemy[1].pos.x = 1280;
	enemy[1].pos.y = 300;
	enemy[1].r = 40;
	enemy[1].life = 1;
	enemy[1].color = GetColor(0, 60, 0);
	media[2] = LoadGraph(".\\media\\bird_aoitori_bluebird.png");
	//��̓G���
	enemy[2].pos.x = 1280;
	enemy[2].pos.y = 200;
	enemy[2].r = 40;
	enemy[2].life = 1;
	enemy[2].color = GetColor(0, 60, 0);
	media[2] = LoadGraph(".\\media\\bird_aoitori_bluebird.png");
	//�w�i
	media[3] = LoadGraph(".\\media\\kajuen_ringo.png");
	//�����S�I�u�W�F�N�g
	object[0].pos.x = 0;
	object[0].pos.y = -30;
	object[0].life = 1;
	object[1].pos.x = 0;
	object[1].pos.y = 130;
	object[1].life = 1;
	object[2].pos.x = 0;
	object[2].pos.y = 300;
	object[2].life = 1;
	media[4] = LoadGraph(".\\media\\fruit_ringo.png");
	//�G���f�B���O
	media[5] = LoadGraph(".\\media\\bluebird_enjou.png");
	media[6] = LoadGraph(".\\media\\nouka_ase_fuku.png");
	//�o�b�g�G���h
	media[7] = LoadGraph(".\\media\\toi.png");
	media[8] = LoadGraph(".\\media\\hansei_koukai_man.png");
	//BGM
	sound[0] = LoadSoundMem(".\\music\\game_maoudamashii_5_village05.mp3"); //�^�C�g��
	PlaySoundMem(sound[0], DX_PLAYTYPE_BACK);
	sound[1] = LoadSoundMem(".\\music\\bgm_maoudamashii_ethnic31.mp3"); //�퓬
	sound[2] = LoadSoundMem(".\\music\\game_maoudamashii_9_jingle09.mp3"); //�G���f�B���O
	sound[3] = LoadSoundMem(".\\music\\game_maoudamashii_9_jingle10.mp3"); //�o�b�g�G���f�B���O
	//���ʉ�
	sound[4] = LoadSoundMem(".\\music\\gun2.mp3"); //�e��
	sound[5] = LoadSoundMem(".\\music\\punch-middle2.mp3"); //�G�U����


	for (i = 0; i < PLAYERMAX_SHOT; i++)
		shot[i][i].life = 0;

	while (ProcessMessage() == 0)
	{
		GetHitKeyStateAll(keyBuf); // ���ׂẴL�[�̏�Ԃ𓾂�
		ClearDrawScreen();

		switch (function_status) {

		case GAME_TITLE:
			function_status = Title();
			PlaySoundMem(sound[1], DX_PLAYTYPE_BACK);
			break;
		case GAME_MAIN:
			function_status = GameMain();
			break;
		case GAME_ENDING:
			function_status = Ending();
			PlaySoundMem(sound[0], DX_PLAYTYPE_BACK);
			break;
		case GAME_BADEND:
			function_status = Badend();
			PlaySoundMem(sound[0], DX_PLAYTYPE_BACK);
			break;
		default:
			DxLib_End(); // �c�w���C�u�����g�p�̏I������
			return 0;
			break;
		}

		if (ProcessMessage() == -1) break; //�G���[���N������I��

		ScreenFlip();

	}
	DxLib_End();
	return 0;
}

