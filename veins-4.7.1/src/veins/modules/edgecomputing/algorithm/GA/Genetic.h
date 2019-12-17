

using namespace std;

#define  PI    3.14159265358979323846

//遗传算法参数，种群规模（0~100）、繁殖代数、函数变量个数、交叉概率、编译概率
# define GROUP_SCALE    50
# define MAX_GENS       500
# define N_VARS         2
# define P_MATING       0.8
# define P_MUTATION     0.15

struct Individual
{
    double Xn[N_VARS];      //存放变量值
    double Fitness;         //适应值
    double ReFitness;       //适应值概率密度
    double SumFitness;      //累加分布，为轮盘转
};
struct X_Range
{
    double Upper;           //变量的上界取值
    double Lower;           //变量的下界取值
};

template<typename T>
T randT(T Lower, T Upper); //产生任意类型随机数函数

void crossover(int &seed);
void elitist();        //基因保留
void evaluate();

void initGroup(int &seed);

void selectBest();
void mutate(int &seed);

double r8_uniform_ab(double a, double b, int &seed);
int i4_uniform_ab(int a, int b, int &seed);

void report(int Xnration);
void selector(int &seed);
void showTime();
void Xover(int one, int two, int &seed);
