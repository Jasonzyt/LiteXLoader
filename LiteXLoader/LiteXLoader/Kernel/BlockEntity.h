#pragma once
#include <string>
#include <vector>

class BlockActor;
class IntVec4;
BlockActor* Raw_GetBlockEntity(IntVec4 pos);
bool Raw_HasBlockEntity(Block* bl);
int Raw_GetBlockEntityType(Block* bl);
unsigned int Raw_GetBlockEntityType(BlockActor* ba);
IntVec4 Raw_GetBlockEntityPos(BlockActor* ba);