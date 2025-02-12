#pragma once

#include "renderer.h"
#include "MovieTexture.h"

#define SAFE_RELEASE(x) if(x) { (x)->Release(); x = nullptr; }


struct VideoTexture {

  static bool createAPI();
  static void destroyAPI();

  struct InternalData;
  InternalData* internal_data = nullptr;

  bool create(const char* filename);
  void destroy();
  bool update(float dt);

  void pause();
  void resume();
  bool hasFinished();
  MovieTexture* getTexture();
  float getAspectRatio() const;
};

