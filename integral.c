#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <getopt.h>

typedef double afunc(double);

extern double f1(double);
extern double f2(double);
extern double f3(double);
double f4(double);
double f5(double);
double f6(double);

double root(afunc *f, afunc *g, double a, double b, double eps1);
double integral(afunc *f, double a, double b, double eps2);

int cmp(const void *, const void *);

// ln(1 + x)
double f4(double x)
{
    return log(1 + x);
}

// x ^ {1/4}
double f5(double x)
{
    return pow(x, 1.0 / 4.0);
}

// sqrt(4 - x^2)
double f6(double x)
{
    return sqrt(4 - x * x);
}

size_t iterations;
const double eps1 = 1e-6;
double root(afunc *f, afunc *g, double a, double b, double eps1)
{
    iterations = 0;

    double Fa = f(a) - g(a);
    double Fb = f(b) - g(b);

    if (Fa * Fb > 0)
    {
        perror("wrong arguments provided: F(a) * F(b) has to be <= 0\n");
        exit(1);
    }

    double c;
    double Fc;
    double Feps;

    do
    {
        c = (a * Fb - b * Fa) / (Fb - Fa);
        Fc = f(c) - g(c);

        if (Fa * Fc > 0.0)
        {
            a = c;
            Fa = Fc;
            Feps = f(c + eps1) - g(c + eps1);
        }
        else
        {
            b = c;
            Fb = Fc;
            Feps = f(c - eps1) - g(c - eps1);
        }

        ++iterations;

    } while (Fc * Feps > 0.0);

    return c;
}

const double p = 1.0 / 15.0;
const double eps2 = 1e-6;
double integral(afunc *f, double a, double b, double eps2)
{
    int n = 10;
    double fa = f(a);
    double fb = f(b);
    double h = (b - a) / (2 * n);

    double sigma1 = 0.0;
    double sigma2 = 0.0;

    for (int i = 0; i < n; ++i)
    {
        if (i)
            sigma1 += f(a + 2 * i * h);

        sigma2 += f(a + (2 * i + 1) * h);
    }

    double I_n;
    double I_2n = h / 3 * (fa + 2 * sigma1 + 4 * sigma2 + fb);

    do
    {
        I_n = I_2n;
        n <<= 1;
        h /= 2.0;
        sigma1 += sigma2;
        sigma2 = 0;

        for (int i = 1; i < n; ++i)
        {
            sigma2 += f(a + (2 * i + 1) * h);
        }

        I_2n = h / 3 * (fa + 2 * sigma1 + 4 * sigma2 + fb);
        // printf("I_n: %lf, I_2n: %lf\np|I_n-I_2n|: %lf\n", I_n, I_2n, p * fabs(I_n - I_2n));

    } while (eps2 < p * fabs(I_n - I_2n));

    // printf("eps=%lf<%lf=p|I_n-I_2n|\n", eps2, p * fabs(I_n - I_2n));
    return I_2n;
}

int cmp(const void *a, const void *b)
{
    double l = *(double *)a;
    double r = *(double *)b;
    if (l < r)
        return -1;
    else if (l > r)
        return 1;
    return 0;
}

int main(int argc, char *argv[])
{
    // store values for root() function in structure
    struct root_unit
    {
        afunc *f;
        afunc *g;
        double L;
        double R;
    } units[] = {
        {f1, f2, -0.5, 3.5},
        {f2, f3, -0.5, 1},
        {f3, f1, -1, 1}};

    size_t N = sizeof(units) / sizeof(struct root_unit);

    // prompt options
    struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"root", no_argument, 0, 'r'},
        {"iterations", no_argument, 0, 'i'},
        {"test-root", required_argument, 0, 'R'},
        {"test-integral", required_argument, 0, 'I'},
        {0, 0, 0, 0}};

    // scan options in loop
    int c;
    while ((c = getopt_long(argc, argv, "hriR:I:", long_options, NULL)) != -1)
    {
        switch (c)
        {
            // variables for test-root & test-integral options
            double A, B, E, R;
            static afunc *funcs[] = {f1, f2, f3, f4, f5, f6};

        case 'h':
            printf("Run without options calculates the area of figure enclosed by task functions\n");
            printf("Supported options:\n");
            printf(
                "  long_opt\t\t|  opt  | \targ\t | description\n"
                "  ----------------------------------------------------\n"
                "  --help\t\t|  -h   | \tNO\t | shows this window\n"
                "  --root\t\t|  -r   | \tNO\t | finds roots of functions given in task\n"
                "  --iterations\t\t|  -i   | \tNO\t | finds number of iterations to find roots of functions given in task\n"
                "  --test-root\t\t|  -R   | F1:F2:A:B:E:R\t | test root() function\n"
                "  --test-integral\t|  -I   |  F:A:B:E:R\t | test integral() function\n\n");
            printf("F, F1, F2 - index of function (1 to 6)\n");
            printf("A, B - borders of segment (double, double)\n");
            printf("E - epsilon, precision (double)\n");
            printf("R - correct answer for tested input (double)\n");

            break;

        case 'r':
            // just print roots
            for (size_t i = 0; i < N; i++)
            {
                printf("%lf ", root(units[i].f, units[i].g, units[i].L, units[i].R, eps1));
            }
            printf("\n");

            break;

        case 'i':
            // sum of iterations finding each root
            size_t S = 0;

            for (size_t i = 0; i < N; i++)
            {
                root(units[i].f, units[i].g, units[i].L, units[i].R, eps1);
                S += iterations;
            }

            printf("%u\n", S);
            break;

        case 'R':
            int F1, F2;

            // parse argument
            sscanf(optarg, "%d:%d:%lf:%lf:%lf:%lf", &F1, &F2, &A, &B, &E, &R);

            // find root
            double X = root(funcs[F1 - 1], funcs[F2 - 1], A, B, E);

            // calculate absolute error
            E = fabs(X - R);

            // print results
            printf("%lf %lf %lf\n", X, E, E / fabs(R));

            break;

        case 'I':
            int F;

            // parse argument
            sscanf(optarg, "%d:%lf:%lf:%lf:%lf", &F, &A, &B, &E, &R);

            // calculate integral
            double I = integral(funcs[F - 1], A, B, E);

            // calculate absolute error
            E = fabs(I - R);

            // print results
            printf("%lf %lf %lf\n", I, E, E / fabs(R));

            break;

        default:
            printf("try \"./integral --help\" for learning supported options\n");
        }
    }

    if (argc == 1)
    {
        struct integral_unit
        {
            double x;
            afunc *f;
            afunc *g;
        } iunits[N];

        // find roots
        for (size_t i = 0; i < N; ++i)
        {
            iunits[i].x = root(units[i].f, units[i].g, units[i].L, units[i].R, eps1);
            iunits[i].f = units[i].f;
            iunits[i].g = units[i].g;
        }
        // sort roots (overkill, just as whole this task)
        qsort(iunits, N, sizeof(struct integral_unit), cmp);

        // I - accumulated area of given figure
        double I = 0;
        if (N == 0)
        {
            printf("%lf\n", I);
            return 0;
        }

        // f & g - borders of figure on current segment
        afunc *f = iunits[0].f;
        afunc *g = iunits[0].g;

        // iterate over segments
        for (size_t i = 1; i < N; ++i)
        {
            // area of new part is |I_f - I_g|
            double I_f = integral(f, iunits[i - 1].x, iunits[i].x, eps2);
            double I_g = integral(g, iunits[i - 1].x, iunits[i].x, eps2);

            // add area of new part of the figure
            I += fabs(I_f - I_g);

            // update functions
            if (f == iunits[i].f)
                f = iunits[i].g;
            else if (f == iunits[i].g)
                f = iunits[i].f;
            else if (g == iunits[i].f)
                g = iunits[i].g;
            else
                g = iunits[i].f;
        }

        printf("%lf\n", I);
    }
    return 0;
}
