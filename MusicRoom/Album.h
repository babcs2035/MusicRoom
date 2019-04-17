// include
#pragma once
#include "Main.h"
#include "Bar.h"

// アルバム詳細画面
class Album : public MyApp::Scene
{
private:

	// const
	const int DEFAULT_albumName_X = 333;
	const int DEFAULT_albumCreator_X = 333;
	const int DEFAULT_albumExpl_Y = 143 + BAR_HEIGHT;
	const int DRAW_STAYMSEC = 3500;
	const int DRAW_MOVE_X_PER_SEC = 48;
	const int DRAW_MOVE_Y_PER_SEC = 12;

	// 曲リスト 構造体
	struct List_detail
	{
		Audio music;
		String displayName, originName, fileName;
		int totalTime;
	};

	// グローバル変数
	std::map<String, std::vector<List_detail>>albums;
	std::vector<List_detail>albumList;
	Array<String> texts;
	Texture main, playing, pausing, not_fav, fav, albumImg;
	Font font_albumName, font_albumCreator, font_albumExpl, font_albumList;
	String albumName = U"", albumBName = U"", albumCreator = U"", albumExpl = U"", selectedAlbumName = U"", selectedAlbumBName = U"", selectedMusicName = U"";
	RoundRect rect_albumImage, rect_albumName, rect_albumCreator, rect_albumExpl, rect_albumList_Flag, rect_albumList_Name, rect_albumList_Time, rect_albumList_Fav, rect_albumListAll, rect_albumListCell;
	Triangle goUp, goDown;
	Audio selectedMusic;
	int64 draw_albumName_startMSec, draw_albumCreator_startMSec, draw_albumExpl_startMSec, draw_albumName_stayMSec, draw_albumCreator_stayMSec, draw_albumExpl_stayMSec;
	int albumList_begin, selectedMusic_num;
	double draw_albumName_x, draw_albumCreator_x, draw_albumExpl_y;
	bool draw_albumName_stayFlag, draw_albumCreator_stayFlag, draw_albumExpl_stayFlag, reloadFlag = false;

public:

	// 初期化
	Album(const InitData& init);

	// 更新
	void update() override;

	// 描画
	void draw() const override;

	// アルバム説明 更新
	void albumExpl_Update();

	// アルバム説明 描画
	void albumExpl_Draw() const;

	// アルバム・曲情報 受け渡し（flag == 1 -> 次 : -1 -> 前）
	void setAlbumMusicName(String& album_Name, String& album_BName, String& musicName, Audio& musicData);
	void setAlbumMusicName(int flag, String& album_Name, String& album_BName, String& musicName, Audio& music);

	// 各文字列 描画
	void Update_drawAlbumDetailStrings();

	// 曲名短縮
	String Detail_musicNameBeShort(String text);

	// 曲操作
	// kind: 0->一時停止, 1->再生, 2->停止
	void setMusicStats(int kind);

	// 曲リスト開始位置 操作
	void changeMusicList_Begin();
};
