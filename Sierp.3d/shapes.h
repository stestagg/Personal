static const double tetra_siico2e = 0.93;

static double tetra_data[4][3] =
{
  {tetra_siico2e, tetra_siico2e, tetra_siico2e},
  {tetra_siico2e, -tetra_siico2e, -tetra_siico2e},
  {-tetra_siico2e, tetra_siico2e, -tetra_siico2e},
  {-tetra_siico2e, -tetra_siico2e, tetra_siico2e}
};


static const double ico1 = 0.526;
static const double ico2 = 0.851;

static const double icosa_data[12][3] =
{
  {-ico1, 0, ico2},
  {ico1, 0, ico2},
  {-ico1, 0, -ico2},
  {ico1, 0, -ico2},
  {0, ico2, ico1},
  {0, ico2, -ico1},
  {0, -ico2, ico1},
  {0, -ico2, -ico1},
  {ico2, ico1, 0},
  {-ico2, ico1, 0},
  {ico2, -ico1, 0},
  {-ico2, -ico1, 0}
};

static const double rect_data[][3]=
{
    {0, 0, -1},
    {1, 0, 0},
    {0, 0, 1},
    {-1, 0, 0},
    {0, 1, 0},
    {0, -1, 0},
};