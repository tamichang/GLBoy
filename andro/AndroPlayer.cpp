#include "AndroPlayer.hpp"
#include "glboy_AndroPlayer.h"


AndroPlayer::AndroPlayer()
{
	LOGV("AndroPlayer constractor\n");
	start = std::chrono::system_clock::now();
	frame = 0;
	boy = glboy::GLBoy::instance;
}

int AndroPlayer::run() {
	//boy->frame_count++;
	boy->render();

	auto end = std::chrono::system_clock::now();       // 計測終了時刻を保存
	auto dur = end - start;        // 要した時間を計算
	auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
	// 要した時間をミリ秒（1/1000秒）に変換して表示
	frame++;
	if(msec > 1000) {
		LOGV("Frame Rate : %d", frame);
		start = end;
		frame = 0;
	}
  	return 0;
}


AndroPlayer::~AndroPlayer()
{
	LOGV("AndroPlayer destractor\n");
}

void AndroPlayer::mouse_position(GLfloat& xpos, GLfloat& ypos)
{
	double x = touchX;
	double y = touchY;

	//GLBoy* glboy = GLBoy::instance;
	//glfwGetCursorPos(window, &x, &y);
	x -= boy->width/2;
	y = -1 * (y - boy->height/2);

	xpos = (GLfloat) x;
	ypos = (GLfloat) y;
}

AndroPlayer* player;
glboy::GLBoy* boy;
bool initial = true;

JNIEXPORT void JNICALL Java_glboy_AndroPlayer_resize (JNIEnv * env, jclass cls, jint width, jint height) {
	if (initial) {
		player = new AndroPlayer();
		boy = glboy::GLBoy::instance;
		boy->init(player, (int)width, (int)height);
		boy->setup();
		initial = false;
	} else {
		boy->size(width, height);
	}
}

JNIEXPORT void JNICALL Java_glboy_AndroPlayer_render (JNIEnv * env, jclass cls) {
	if(!initial)
		player->run();
}


JNIEXPORT void JNICALL Java_glboy_AndroPlayer_touch (JNIEnv * env, jclass cls, jfloat x, jfloat y) {
	player->touchX = (float) x;
	player->touchY = (float) y;
	//LOGV("touchX %f, touchY %f", player->touchY, player->touchY);
}