#pragma once
#include "common.h"
#include "ReduceError.h"
#include "GenerateObj.h"
#include "veinReconstruction.h"
void processing3_26();
void processing4_4();
void processingTwoImage();

string ArteryReduceErrorAndGenerateMesh(string in_file, string out_file);
void VeinReduceErrorAndGenerateMesh(string in_file, string vein_reducer_outfile);
void wholeProcessing(string video);
//void ArteryReduceErrorAndGenerateMesh(string in_file, string out_file);