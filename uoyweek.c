#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

const char DAYS[7][10] = {
    "sunday",
    "monday",
    "tuesday",
    "wednesday",
    "thursday",
    "friday",
    "saturday"};

time_t normalise_week_to_monday(time_t to_normalise)
{
    struct tm tm_to_normalise;
    gmtime_s(&tm_to_normalise, &to_normalise);

    time_t result = to_normalise;
    result -= tm_to_normalise.tm_sec;
    result -= tm_to_normalise.tm_min * 60;
    result -= tm_to_normalise.tm_hour * 3600;
    result -= (tm_to_normalise.tm_wday - 1) * 24 * 3600;

    return result;
}

typedef struct Term
{
    time_t start_time_stamp;
    time_t end_time_stamp;
    struct tm start;
    struct tm end;
    char *code_name;
} Term;

Term *term_new(char *c, time_t s, time_t e)
{
    Term *new_term = (Term *)malloc(sizeof(Term));
    if (new_term == NULL)
        return NULL;

    new_term->code_name = (char *)malloc(sizeof(char) * 4);
    if (new_term->code_name == NULL)
        return NULL;
    new_term->code_name = c;

    new_term->start_time_stamp = s;
    new_term->end_time_stamp = e;

    struct tm start;
    gmtime_s(&start, &s);

    struct tm end;
    gmtime_s(&end, &e);

    new_term->start = start;
    new_term->end = end;

    return new_term;
}

int term_contains_time(Term *term, time_t ts)
{
    return term->start_time_stamp <= ts && ts <= term->end_time_stamp;
}

char *term_to_string(Term *term)
{
    char *term_string = (char *)malloc(sizeof(char) * 128);

    char start_string[64];
    asctime_s(start_string, 64, &term->start);

    char end_string[64];
    asctime_s(end_string, 64, &term->end);

    snprintf(term_string, 128, "%s (%d-%d) %s %s", term->code_name, ((&term->start)->tm_year + 1900), ((&term->end)->tm_year + 1900), start_string, end_string);

    return term_string;
}

int term_get_week(Term *term, time_t term_time)
{
    time_t start_normalised = normalise_week_to_monday(term->start_time_stamp);
    time_t interval = term_time - start_normalised;
    int weeks = interval / 604800ll;
    return weeks + 1;
}

typedef struct Terms
{
    Term **terms;
    size_t term_count;
} Terms;

Terms *terms_new()
{
    Terms *terms_new = malloc(sizeof(Terms));
    if (terms_new == NULL)
        return NULL;

    terms_new->term_count = 0;
    terms_new->terms = NULL;

    return terms_new;
}

int terms_add(Terms *terms, Term *new_term)
{
    terms->term_count += 1;

    if (terms->terms == NULL)
        terms->terms = (Term **)malloc(sizeof(Term **) * terms->term_count);
    else
        terms->terms = (Term **)realloc(terms->terms, sizeof(Term **) * terms->term_count);
    if (terms->terms == NULL)
        return 1;

    (terms->terms)[terms->term_count - 1] = new_term;

    return 0;
}

Term *terms_get_term_from_time(Terms *terms, time_t term_time)
{
    int term_counter = 0;
    while (term_counter < terms->term_count)
    {
        if (term_contains_time(terms->terms[term_counter], term_time))
            return terms->terms[term_counter];
        term_counter++;
    }
    return NULL;
}

char *terms_get_term_string(Terms *terms, time_t term_time, int fancy_mode)
{
    Term *term = terms_get_term_from_time(terms, term_time);
    if (term == NULL)
        return NULL;

    int week = term_get_week(term, term_time);

    struct tm gm_now;
    gmtime_s(&gm_now, &term_time);

    char day[10];
    strncpy_s(day, 10, DAYS[gm_now.tm_wday], 10);

    char term_code[4];
    strncpy_s(term_code, 4, term->code_name, 4);

    if(fancy_mode){
        term_code[0] = toupper(term_code[0]);
        day[0] = toupper(day[0]);
    }

    char *term_time_string = (char *)malloc(sizeof(char) * 17);
    snprintf(term_time_string, 17, "%s/%d/%s", term_code, week, day);

    return term_time_string;
}

int main(int argc, char *argv[])
{
    int fancy = argc == 2 && strncmp(argv[1], "--fancy", 8) == 0;

    time_t now = time(0);
    Term *aut = term_new("aut", (time_t)1506297600l, (time_t)1512086400l);
    if (aut == NULL)
    {
        fprintf(stderr, "Could not create autumn term.\n");
        return 1;
    }

    Term *spr = term_new("spr", (time_t)1515369600l, (time_t)1521158400l);
    if (spr == NULL)
    {
        fprintf(stderr, "Could not create spring term.\n");
        return 2;
    }

    Terms *terms = terms_new();
    if (terms == NULL)
    {
        fprintf(stderr, "Could not create terms.\n");
        return 3;
    }

    if (terms_add(terms, aut))
    {
        fprintf(stderr, "Could not add autumn term to terms.\n");
        return 4;
    }
    if (terms_add(terms, spr))
    {
        fprintf(stderr, "Could not add spring term to terms.\n");
        return 5;
    }

    char *term_string = terms_get_term_string(terms, now, fancy);
    if (term_string == NULL)
    {
        fprintf(stderr, "Could get term string.\n");
        return 6;
    }
    printf("%s", term_string);

    return 0;
}