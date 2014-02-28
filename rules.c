/* copyright 2013 Sascha Kruse and contributors (see LICENSE for licensing information) */

#include <glib.h>
#include <fnmatch.h>
#include <regex.h>

#include "dunst.h"
#include "rules.h"
#include "settings.h"

/*
 * Apply rule to notification.
 */
void rule_apply(rule_t * r, notification * n)
{
        if (r->timeout != -1)
                n->timeout = r->timeout;
        if (r->urgency != -1)
                n->urgency = r->urgency;
        if (r->fg)
                n->color_strings[ColFG] = r->fg;
        if (r->bg)
                n->color_strings[ColBG] = r->bg;
        if (r->format)
                n->format = r->format;
        if (r->script)
                n->script = r->script;
}

/*
 * Check all rules if they match n and apply.
 */
void rule_apply_all(notification * n)
{
        for (GSList * iter = rules; iter; iter = iter->next) {
                rule_t *r = iter->data;
                if (rule_matches_notification(r, n)) {
                        rule_apply(r, n);
                }
        }
}

/*
 * Initialize rule with default values.
 */
void rule_init(rule_t * r)
{
        r->name = NULL;
        r->appname = NULL;
        r->summary = NULL;
        r->body = NULL;
        r->icon = NULL;
        r->timeout = -1;
        r->urgency = -1;
        r->fg = NULL;
        r->bg = NULL;
        r->format = NULL;
}

/*
 * Check whether rule should be applied to notification.
 */
bool regex_match();
bool rule_matches_notification(rule_t * r, notification * n)
{
        if (settings.regex_rules)
                return ((!r->appname || !regex_match(r->appname, n->appname))
                        && (!r->summary || !regex_match(r->summary, n->summary))
                        && (!r->body || !regex_match(r->body, n->body))
                        && (!r->icon || !regex_match(r->icon, n->icon)));

        return ((!r->appname || !fnmatch(r->appname, n->appname, 0))
                && (!r->summary || !fnmatch(r->summary, n->summary, 0))
                && (!r->body || !fnmatch(r->body, n->body, 0))
                && (!r->icon || !fnmatch(r->icon, n->icon, 0)));
}
/* returns 0 if regex_string matches match_string */
bool regex_match(char *regex_string, char *match_string)
{
        regex_t regex;
        int reti;

        regcomp(&regex, regex_string, 0);
        reti = regexec(&regex, match_string, 0, NULL, 0);
        regfree(&regex);

        return reti;
}
/* vim: set ts=8 sw=8 tw=0: */
