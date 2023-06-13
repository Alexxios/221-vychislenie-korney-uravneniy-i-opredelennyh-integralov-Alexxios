#include <stdio.h>
#include <math.h>
#include <getopt.h>

typedef double afunc(double);

extern double f1(double);
extern double f2(double);
extern double f3(double);

double root(afunc *f, afunc *g, double a, double b, double eps1);
double integral(afunc *f, double a, double b, double eps2);

double root(afunc *f, afunc *g, double a, double b, double eps1)
{
    return 0;
}

const double p = 1.0 / 15.0;
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
    int c;
    // int digit_optind = 0;

    int F, F2;
    double A, B, E, R;

    while (1)
    {
        // int this_option_optind = optind ? optind : 1;
        int option_index = 0;
        static struct option long_options[] = {
            {"help", no_argument, 0, 'h'},
            {"root", no_argument, 0, 'r'},
            {"iterations", no_argument, 0, 'i'},
            {"test-root", required_argument, 0, 'R'},
            {"test-integral", required_argument, 0, 'I'},
            {0, 0, 0, 0}};

        c = getopt_long(argc, argv, "hriR:I:",
                        long_options, &option_index);
        if (c == -1)
            break;

        switch (c)
        {
        case 0:
            // TODO: root(f1, f2), root(f1, f3), root(f2, f3), integral(...)
            printf("TBA\n");
            break;

        case 'h':
            printf(
                "\t--help\t\t-h\tshows this window\n"
                "\t--root\t\t-r\tTBA\n"
                "\t--iterations\t-i\tTBA\n"
                "\t--test-root\t-R\tTBA\n"
                "\t--test-integral\t-I\ttest integral() function\n");
            break;

        case 'r':
            // TODO: root(f1, f2), root(f1, f3), root(f2, f3)
            printf("TBA\n");
            break;

        case 'i':
            // TODO: #root(f1, f2) + #root(f1, f3) + #root(f2, f3)
            printf("TBA\n");
            break;

        case 'R':
            sscanf(optarg, "%d:%d:%lf:%lf:%lf:%lf", &F, &F2, &A, &B, &E, &R);

            // TODO: root(F1, F2, A, B, E)
            printf("TBA\n");
            break;

        case 'I':
            sscanf(optarg, "%d:%lf:%lf:%lf:%lf", &F, &A, &B, &E, &R);

            double I = 0.0;
            switch (F)
            {
            case 1:
                I = integral(f1, A, B, E);
                break;
            case 2:
                I = integral(f2, A, B, E);
                break;
            case 3:
                I = integral(f3, A, B, E);
                break;
            default:
                break;
            }

            printf("%d\n[%lf, %lf]\neps=%lf\nres=%lf, real=%lf\n", F, A, B, E, I, R);
            break;

            // case '0':
            // case '1':
            // case '2':
            //     if (digit_optind != 0 && digit_optind != this_option_optind)
            //         printf("digits occur in two different argv-elements.\n");
            //     digit_optind = this_option_optind;
            //     printf("option %c\n", c);
            //     break;

            // case 'a':
            //     printf("option a\n");
            //     break;

            // case 'b':
            //     printf("option b\n");
            //     break;

            // case 'c':
            //     printf("option c with value '%s'\n", optarg);
            //     break;

            // case 'd':
            //     printf("option d with value '%s'\n", optarg);
            //     break;

            // case '?':
            //     break;

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

    return 0;
}
