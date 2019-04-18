// include
#include <Siv3D.hpp>
#include <HamFramework.hpp>
#include "Bar.h"

// �X�e�[�^�X�o�[

// ������
Bar::Bar(const InitData& init) : IScene(init)
{
	// �{�^�� ������
	originPlay[0] = Texture(U"data\\Bar\\play\\normal.png");
	originPlay[1] = Texture(U"data\\Bar\\play\\active.png");
	originBrief[0] = Texture(U"data\\Bar\\brief\\normal.png");
	originBrief[1] = Texture(U"data\\Bar\\brief\\active.png");
	originStop[0] = Texture(U"data\\Bar\\stop\\normal.png");
	originStop[1] = Texture(U"data\\Bar\\stop\\active.png");
	originSeek[0] = Texture(U"data\\Bar\\seek\\normal.png");
	originSeek[1] = Texture(U"data\\Bar\\seek\\active.png");
	originRep[0] = Texture(U"data\\Bar\\rep\\normal.png");
	originRep[1] = Texture(U"data\\Bar\\rep\\active.png");
	originPrev[0] = Texture(U"data\\Bar\\prev\\normal.png");
	originPrev[1] = Texture(U"data\\Bar\\prev\\active.png");
	originNext[0] = Texture(U"data\\Bar\\next\\normal.png");
	originNext[1] = Texture(U"data\\Bar\\next\\active.png");
	originBack[0] = Texture(U"data\\Bar\\Back\\normal.png");
	originBack[1] = Texture(U"data\\Bar\\Back\\active.png");
	originGo[0] = Texture(U"data\\Bar\\Go\\normal.png");
	originGo[1] = Texture(U"data\\Bar\\Go\\active.png");
	originShare[0] = Texture(U"data\\Bar\\Share\\normal.png");
	originShare[1] = Texture(U"data\\Bar\\Share\\active.png");
	displayPlay = originPlay[0];
	displayBrief = originBrief[0];
	displayStop = originStop[0];
	displaySeek = originSeek[0];
	displayRep = originRep[0];
	displayPrev = originPrev[0];
	displayNext = originNext[0];
	displayBack = originBack[0];
	displayGo = originGo[0];
	displayShare = originShare[0];
	mainRect = RoundRect(192, 0, mainRectWidth, BAR_HEIGHT, 16);
	fieldRect = Rect(0, 0, GameInfo::Width, GameInfo::Height);
	mainFont = Font(18);
	timeFont = Font(12);
	dog = Audio(U"data\\Bar\\dog.mp3");
}

// �X�V
void Bar::update()
{

	if (!music.music.isEmpty() && !music.music.isPlaying() && !stop_flag
		&& music.music.samplesPlayed() % music.music.samples() == 0)
	{
		if (get_prevScene() == Scene_Fav || get_nowScene() == Scene_Fav) setFavMusicName(1, music.albumName, hoge, music.text, music.music);
		else changeMusic(1);
	}
	if (KeyShift.pressed() && KeyD.pressed() && KeyO.pressed() && KeyG.pressed()) dog.play();

	// �{�^�� �X�V
	const Rect backRect(10, 10, 44, 44);
	const Rect goRect(GameInfo::Width - 54, 10, 44, 44);
	switch (get_nowScene())
	{
	case Scene_Select:
		draw_back_flag = draw_go_flag = false;
		break;

	case Scene_Detail:
		draw_back_flag = true;
		draw_go_flag = music.music.isPlaying();
		if (backRect.leftClicked)
		{
			music.music.stop();
			stop_flag = true;
			changeScene(U"Album");
		}
		if (draw_go_flag && goRect.leftClicked) changeScene(U"Music");
		break;

	case Scene_Music:
		draw_back_flag = true;
		draw_go_flag = false;
		if (backRect.leftClicked)
		{
			changeScene((get_prevScene() == Scene_Fav || isFavLooping() ? Scene_Fav : Scene_Detail));
		}
		break;

	case Scene_Fav:
		draw_back_flag = true;
		draw_go_flag = music.music.isPlaying();
		if (backRect.leftClicked)
		{
			music.music.stop();
			stop_flag = true;
			changeScene(U"Select");
		}
		if (draw_go_flag && goRect.leftClicked)
		{
			changeScene(U"Music");
			setFavMusicName(0, music.albumName, hoge, music.text, music.music);
		}
		break;
	}
	displayBack = originBack[(backRect.mouseOver ? 1 : 0)];
	displayGo = originGo[(goRect.mouseOver ? 1 : 0)];

	if (music.music.isPaused() || music.music.isPlaying())
	{
		int x = 768 / 2 - mainRectWidth / 2 - 40 * 3;
		for (int cou = 0; cou < 6; ++cou)
		{
			const Circle button(x + 20, 12 + 20, 20);
			switch (cou)
			{
			case 0:
				if (button.mouseOver) displayPrev = originPrev[1];
				else displayPrev = originPrev[0];
				if (button.leftClicked)
				{
					changeMusicStats(0);
					setMusicStats(0);
					changeFavMusicStats(0);
					changeMusic(-1);
					stop_flag = false;
				}
				break;
			case 1:
				if (music.music.isPlaying())
				{
					if (button.mouseOver) displayBrief = originBrief[1];
					else displayBrief = originBrief[0];
					if (button.leftClicked)
					{
						(get_nowScene() == Scene_Detail ? setMusicStats(0) : changeMusicStats(0));
						stop_flag = false;
					}
				}
				else
				{
					if (button.mouseOver) displayPlay = originPlay[1];
					else displayPlay = originPlay[0];
					if (button.leftClicked)
					{
						(get_nowScene() == Scene_Detail ? setMusicStats(1) : changeMusicStats(1));
						stop_flag = false;
					}
				}
				break;
			case 2:
				if (button.mouseOver) displayRep = originRep[1];
				else displayRep = originRep[0];
				if (musicLoopFlag) displayRep = originRep[1];
				if (button.leftClicked)
				{
					const int tmpTime = (int)music.music.streamPosSample();
					music.music.pause();
					music.music.setLoop(!musicLoopFlag);
					musicLoopFlag = !musicLoopFlag;
					music.music.play();
					music.music.setPosSample(tmpTime);
					stop_flag = false;
				}
				x += mainRectWidth;
				break;
			case 3:
				if (button.mouseOver) displayStop = originStop[1];
				else displayStop = originStop[0];
				if (button.leftClicked)
				{
					changeMusicStats(2);
					stop_flag = true;
				}
				break;
			case 4:
				if (button.mouseOver) displayShare = originShare[1];
				else displayShare = originShare[0];
				if (button.leftClicked) Twitter::OpenTweetWindow(U"#MusicRoom v3.0 �ŃA���o���w" + music.albumName + U"�x�̋ȁu" + music.text + U"�v�𒮂��Ă��܂��I�_�E�����[�h�͂����炩��Fhttps://github.com/Bwambocos/MusicRoom_Siv3D/releases");
				break;
			case 5:
				if (button.mouseOver) displayNext = originNext[1];
				else displayNext = originNext[0];
				if (button.leftClicked)
				{
					changeMusicStats(0);
					setMusicStats(0);
					changeFavMusicStats(0);
					changeMusic(1);
					stop_flag = false;
				}
				break;
			}
			x += 40;
		}
	}

	// ���C���e�L�X�g �X�V
	if (!music.music.isPlaying())
	{
		auto nowScene = get_nowScene();
		switch (nowScene)
		{
		case Scene_Select:
			mainText = U"�A���o����I�����Ă�������";
			break;
		case Scene_Detail:
			mainText = U"�Ȃ�I�����Ă�������";
			break;
		case Scene_Fav:
			mainText = U"�Ȃ�I�����Ă�������";
			break;
		}
	}
	else mainText = U"�w" + music.albumName + U"�x" + music.text;

	Update_drawMainText();
}

// �`��
void Bar::draw() const
{
	fieldRect.draw(Color(204, 61, 104));
	mainRect.draw(Color(224, 67, 114));

	// �{�^�� �`��
	if (draw_back_flag) displayBack.draw(10, 10);
	if (draw_go_flag) displayGo.draw(GameInfo::Width - 10 - displayGo.width, 10);
	int x = 768 / 2 - mainRectWidth / 2 - 40 * 3;
	for (int cou = 0; cou < 6; ++cou)
	{
		switch (cou)
		{
		case 0:
			displayPrev.draw(x, 12);
			break;
		case 1:
			if (music.music.isPlaying()) displayBrief.draw(x, 12);
			else displayPlay.draw(x, 12);
			break;
		case 2:
			displayRep.draw(x, 12);
			x += mainRectWidth;
			break;
		case 3:
			displayStop.draw(x, 12);
			break;
		case 4:
			displayShare.draw(x, 12);
			break;
		case 5:
			displayNext.draw(x, 12);
			break;
		}
		x += 40;
	}

	// ���C���e�L�X�g �`��
	RasterizerState rasterizer = RasterizerState::Default2D;
	rasterizer.scissorEnable = true;
	Graphics2D::SetRasterizerState(rasterizer);
	Graphics2D::SetScissorRect(Rect((int)mainRect.x, (int)mainRect.y, (int)mainRect.w, (int)mainRect.h));
	mainFont(mainText).draw(draw_mainText_x, 15);
	Graphics2D::SetScissorRect(Rect(0, 0, Window::Width(), Window::Height()));
}

// �Đ������擾
bool Bar::is_nowMusicPlaying()
{
	return nowMusic.isPlaying();
}

// �ȏڍ׃f�[�^�󂯓n��
void Bar::giveMusicData(String albumName, String musicName, Audio musicData)
{
	music.albumName = albumName;
	music.text = musicName;
	music.music = musicData;
	stop_flag = false;

	// �`��ʒu ������
	draw_mainText_startMSec = (int)Time::GetMillisec();
	draw_mainText_stayFlag = true;
	draw_mainText_x = DEFAULT_mainText_X;
}

// ��~�t���O �ݒ�
void Bar::set_stopFlag(bool flag)
{
	stop_flag = flag;
}

// �Ȗ��`��ʒu �X�V
void Bar::Update_drawMainText()
{
	auto rect = mainRect;
	auto width = mainFont(mainText).region().w + rect.r;
	if (width > rect.w)
	{
		if (!draw_mainText_stayFlag)
		{
			if (draw_mainText_x + width > rect.x + rect.w) draw_mainText_x -= (double)DRAW_mainText_MOVE_X * (Time::GetMillisec() - draw_mainText_stayMSec) / 1000;
			else
			{
				draw_mainText_startMSec = draw_mainText_stayMSec = (int)Time::GetMillisec();
				draw_mainText_stayFlag = true;
			}
		}
		if (draw_mainText_stayFlag)
		{
			if (draw_mainText_stayMSec - draw_mainText_startMSec >= BAR_DRAW_STAYMSEC)
			{
				draw_mainText_startMSec = draw_mainText_stayMSec;
				const Rect tmpRect = mainFont(mainText).region();
				if (draw_mainText_x == DEFAULT_mainText_X) draw_mainText_stayFlag = false;
				else draw_mainText_x = DEFAULT_mainText_X;
			}
		}
		draw_mainText_stayMSec = (int)Time::GetMillisec();
	}
	else
	{
		const Rect tempRect = mainFont(mainText).region();
		draw_mainText_x = 384 - (int)tempRect.w / 2;
	}
}