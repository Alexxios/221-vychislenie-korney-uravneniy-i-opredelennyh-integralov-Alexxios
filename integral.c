#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <getopt.h>

typedef double afunc(double);

extern double f1(double);
extern double f2(double);
extern double f3(double);

double root(afunc *f, afunc *g, double a, double b, double eps1);
double integral(afunc *f, double a, double b, double eps2);

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

int main(int argc, char *argv[])
{
    // TBA
    struct unit
    {
        afunc *f;
        afunc *g;
        double L;
        double R;
    } units[] = {
        {f1, f2, -0.5, 3.5},
        {f2, f3, -0.5, 1},
        {f3, f1, -1, 1}};

    size_t N = sizeof(units) / sizeof(struct unit);

    // prompt options
    struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"root", no_argument, 0, 'r'},
        {"iterations", no_argument, 0, 'i'},
        {"test-root", required_argument, 0, 'R'},
        {"test-integral", required_argument, 0, 'I'},
        {0, 0, 0, 0}};

    int c;
    while ((c = getopt_long(argc, argv, "hriR:I:", long_options, NULL)) != -1)
    {
        switch (c)
        {
            // variables for test-root & test-integral options
            double A, B, E, R;
            // afunc *funcs[] = {f1, f2, f3};

        case 'h':
            printf(
                "\t--help\t\t-h\tshows this window\n"
                "\t--root\t\t-r\tTBA\n"
                "\t--iterations\t-i\tTBA\n"
                "\t--test-root\t-R\tTBA\n"
                "\t--test-integral\t-I\ttest integral() function\n");
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
            double X = root(units[F1 - 1].f, units[F2 - 1].f, A, B, E);

            // calculate absolute error
            E = fabs(X - R);

            // print results
            printf("%lf %lf %lf\n", X, E, E / fabs(R));

            // if (R != 0.0)
            //     printf("%lf %lf %lf\n", X, D, D / R);
            // else
            //     printf("%lf %lf\n", X, D);
            break;

        case 'I':
            int F;

            // parse argument
            sscanf(optarg, "%d:%lf:%lf:%lf:%lf", &F, &A, &B, &E, &R);

            // calculate integral
            double I = integral(units[F - 1].f, A, B, E);

            // calculate absolute error
            E = fabs(I - R);

            // print results
            printf("%lf %lf %lf\n", I, E, E / fabs(R));

            break;

        default:
            printf("?? getopt returned character code 0%o ??\n", c);
        }
    }

    if (optind < argc)
    {
        printf("non-option ARGV-elements: ");
        while (optind < argc)
            printf("%s ", argv[optind++]);
        printf("\n");
    }
    if (argc == 1)
    {
        // TODO: root(f1, f2), root(f1, f3), root(f2, f3), integral(...)
        printf("TBA\n");
    }
    return 0;
}
