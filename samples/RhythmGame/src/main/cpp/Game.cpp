/*
 * Copyright 2018 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <utils/logging.h>
#include <thread>

#include "Game.h"

Game::Game(AAssetManager *assetManager): mAssetManager(assetManager) {
}

void Game::start() {
    // TODO: Add your code here
    mClap = SoundRecording::loadFromAssets(mAssetManager, "CLAP.raw");

    AudioStreamBuilder builder;
    builder.setFormat(AudioFormat::I16);
    builder.setChannelCount(2);
    builder.setSampleRate(48000);

    builder.setCallback(this);

    builder.setPerformanceMode(PerformanceMode::LowLatency);
    builder.setSharingMode(SharingMode::Exclusive);

    // Open the stream
    Result result = builder.openStream(&mAudioStream);
    if(result != Result::OK){
        LOGE("Failed to open stream. Error: %s", convertToText(result));
    }

    // Reduce stream latency by setting the buffer size to a multiple of the burst size
    mAudioStream->setBufferSizeInFrames(mAudioStream->getFramesPerBurst() * 2);

    // Start the stream
    result = mAudioStream->requestStart();
    if(result != Result::OK){
        LOGE("Failed to start stream. Error: %s", convertToText(result));
    }
}

void Game::tap(int64_t eventTimeAsUptime) {
    // TODO: Add your code here
    mClap->setPlaying(true);
}

void Game::tick(){
    // TODO: Add your code here
}

void Game::onSurfaceCreated() {
    SetGLScreenColor(kScreenBackgroundColor);
}

void Game::onSurfaceChanged(int widthInPixels, int heightInPixels) {
}

void Game::onSurfaceDestroyed() {
}

DataCallbackResult Game::onAudioReady(AudioStream *oboeStream, void *audioData, int32_t numFrames) {
    mClap->renderAudio(static_cast<int16_t *>(audioData), numFrames);
    return DataCallbackResult::Continue;
}
