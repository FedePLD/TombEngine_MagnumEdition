#pragma once

struct BOUNDING_BOX;
struct MATRIX3D;
struct PoseData;
struct Vector3i;

constexpr auto FPS = 30;

constexpr auto ONE_DEGREE = 182;
constexpr auto PREDICTIVE_SCALE_FACTOR = 14;
constexpr auto WALL_SIZE = 1024;
constexpr auto WALL_MASK = WALL_SIZE - 1;
constexpr auto STEP_SIZE = WALL_SIZE / 4;
constexpr auto STOP_SIZE = WALL_SIZE / 2;
constexpr auto GRID_SNAP_SIZE = STEP_SIZE / 2;
constexpr auto STEPUP_HEIGHT = ((STEP_SIZE * 3) / 2);
constexpr auto SWIM_DEPTH = 730;
constexpr auto WADE_DEPTH = STEPUP_HEIGHT;
constexpr auto SHALLOW_WATER_START_LEVEL = STEP_SIZE / 4;
constexpr auto BAD_JUMP_CEILING = ((STEP_SIZE * 3) / 4);
constexpr auto SLOPE_DIFFERENCE = 60;

constexpr auto CLICK = [](auto x) { return STEP_SIZE * x; };
constexpr auto SECTOR = [](auto x) { return WALL_SIZE * x; };

short ANGLE(float angle);
short FROM_DEGREES(float angle);
short FROM_RAD(float angle);
float TO_DEGREES(short angle);
float TO_RAD(short angle);

BoundingOrientedBox TO_DX_BBOX(PoseData pos, BOUNDING_BOX* box);

float phd_sin(short a);
float phd_cos(short a);

const Vector3 GetRandomVector();
const Vector3 GetRandomVectorInCone(const Vector3& direction, const float angleDegrees);
int mGetAngle(int x1, int y1, int x2, int y2);
int phd_atan(int dz, int dx);
void phd_RotBoundingBoxNoPersp(PoseData* pos, BOUNDING_BOX* bounds, BOUNDING_BOX* tbounds);

void InterpolateAngle(short angle, short* rotation, short* outAngle, int shift);
void GetMatrixFromTrAngle(Matrix* matrix, short* framePtr, int index);

constexpr auto FP_SHIFT = 16;
constexpr auto FP_ONE = (1 << FP_SHIFT);
constexpr auto W2V_SHIFT = 14;

void FP_GetMatrixAngles(MATRIX3D* m, short* angles);
