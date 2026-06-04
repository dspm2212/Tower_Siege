/*
 * Variant:  Tower Siege
 * Student:  Daniel Santiago Perez Madera - 20231020203
 *
 * JsonIO.cpp
 * ----------
 * Hand-rolled JSON parser and serialiser -- no external libraries.
 *
 * Parser strategy: find key strings, then scan forward for the value.
 * Handles integers, quoted strings, nested objects, and flat arrays of
 * objects. Does NOT handle escape sequences beyond what the game needs.
 *
 * Serialiser: builds the JSON string with an ostringstream.
 */

#include "JsonIO.h"
#include <fstream>
#include <sstream>
#include <cctype>

/* -------------------------------------------------------------------------
 * Internal parser helpers (static)
 * ---------------------------------------------------------------------- */

/* Trim leading/trailing whitespace */
static std::string trim(const std::string &s) {
    std::size_t b = 0;
    while (b < s.size() && std::isspace((unsigned char)s[b])) b++;
    std::size_t e = s.size();
    while (e > b && std::isspace((unsigned char)s[e - 1])) e--;
    return s.substr(b, e - b);
}

/*
 * Find the integer value for `key` anywhere in `src`.
 * Scans for "key" : <sign><digits>.
 * Returns true and sets `out` if found.
 */
static bool parse_int(const std::string &src, const std::string &key, int &out) {
    std::size_t pos = src.find(key);
    if (pos == std::string::npos) return false;
    pos = src.find(':', pos + key.size());
    if (pos == std::string::npos) return false;
    pos++;
    while (pos < src.size() && std::isspace((unsigned char)src[pos])) pos++;
    int sign = 1;
    if (pos < src.size() && src[pos] == '-') { sign = -1; pos++; }
    int val = 0; bool found = false;
    while (pos < src.size() && std::isdigit((unsigned char)src[pos])) {
        val = val * 10 + (src[pos] - '0');
        found = true; pos++;
    }
    if (!found) return false;
    out = val * sign;
    return true;
}

/*
 * Find a quoted string value for `key` anywhere in `src`.
 * Scans for "key" : "value".
 */
static bool parse_string(const std::string &src, const std::string &key, std::string &out) {
    std::size_t pos = src.find(key);
    if (pos == std::string::npos) return false;
    pos = src.find(':', pos + key.size());
    if (pos == std::string::npos) return false;
    pos = src.find('"', pos + 1);
    if (pos == std::string::npos) return false;
    pos++;                              /* skip opening quote */
    std::size_t end = src.find('"', pos);
    if (end == std::string::npos) return false;
    out = src.substr(pos, end - pos);
    return true;
}

/*
 * Extract the JSON array string for `key` (everything between '[' and ']').
 * Returns empty string if not found.
 */
static std::string extract_array(const std::string &src, const std::string &key) {
    std::size_t kpos = src.find(key);
    if (kpos == std::string::npos) return "";
    std::size_t open = src.find('[', kpos + key.size());
    if (open == std::string::npos) return "";
    /* Find matching ']' -- not nested arrays in our schema */
    std::size_t close = src.find(']', open);
    if (close == std::string::npos) return "";
    return src.substr(open + 1, close - open - 1);
}

/*
 * Split a flat JSON array body (between '[' and ']') into individual
 * object strings. Handles one level of nesting only.
 */
static std::vector<std::string> split_objects(const std::string &arr) {
    std::vector<std::string> out;
    std::size_t pos = 0;
    while (pos < arr.size()) {
        std::size_t open = arr.find('{', pos);
        if (open == std::string::npos) break;
        std::size_t close = arr.find('}', open);
        if (close == std::string::npos) break;
        out.push_back(trim(arr.substr(open, close - open + 1)));
        pos = close + 1;
    }
    return out;
}

/*
 * Extract the body of a balanced [...] for `key`, keeping nested brackets.
 * Used for "paths", which is an array of arrays.
 */
static std::string extract_balanced_array(const std::string &src, const std::string &key) {
    std::size_t kpos = src.find(key);
    if (kpos == std::string::npos) return "";
    std::size_t open = src.find('[', kpos + key.size());
    if (open == std::string::npos) return "";
    int depth = 0;
    for (std::size_t i = open; i < src.size(); i++) {
        if (src[i] == '[') depth++;
        else if (src[i] == ']' && --depth == 0)
            return src.substr(open + 1, i - open - 1);
    }
    return "";
}

/* Split an array body into its balanced [...] sub-array strings */
static std::vector<std::string> split_arrays(const std::string &body) {
    std::vector<std::string> out;
    std::size_t i = 0;
    while (i < body.size()) {
        if (body[i] != '[') { i++; continue; }
        int depth = 0;
        std::size_t start = i;
        for (; i < body.size(); i++) {
            if (body[i] == '[') depth++;
            else if (body[i] == ']' && --depth == 0) { i++; break; }
        }
        out.push_back(body.substr(start, i - start));
    }
    return out;
}

/* -------------------------------------------------------------------------
 * Public API -- file I/O
 * ---------------------------------------------------------------------- */

bool read_file(const std::string &path, std::string &out) {
    std::ifstream f(path);
    if (!f.is_open()) return false;
    std::ostringstream buf;
    buf << f.rdbuf();
    out = buf.str();
    return true;
}

bool write_file(const std::string &path, const std::string &content) {
    std::ofstream f(path);
    if (!f.is_open()) return false;
    f << content;
    return true;
}

/* -------------------------------------------------------------------------
 * Public API -- parsing input.json
 * ---------------------------------------------------------------------- */

bool parse_input(const std::string &text, GameInput &input) {
    /* command */
    if (!parse_string(text, "\"command\"", input.command)) input.command = "step";

    /* grid */
    if (!parse_int(text, "\"rows\"", input.rows))   input.rows = 8;
    if (!parse_int(text, "\"cols\"", input.cols))   input.cols = 8;

    /* castle */
    std::size_t castle_pos = text.find("\"castle\"");
    if (castle_pos != std::string::npos) {
        std::size_t obj_open  = text.find('{', castle_pos);
        std::size_t obj_close = text.find('}', obj_open);
        if (obj_open != std::string::npos && obj_close != std::string::npos) {
            std::string castle_obj = text.substr(obj_open, obj_close - obj_open + 1);
            parse_int(castle_obj, "\"row\"", input.castle_row);
            parse_int(castle_obj, "\"col\"", input.castle_col);
        }
    }
    if (!parse_int(text, "\"castle_hp\"", input.castle_hp)) input.castle_hp = 100;

    /* towers array */
    input.towers.clear();
    std::string towers_arr = extract_array(text, "\"towers\"");
    for (const std::string &obj : split_objects(towers_arr)) {
        Tower t{};
        parse_int(obj, "\"id\"",           t.id);
        parse_int(obj, "\"row\"",          t.row);
        parse_int(obj, "\"col\"",          t.col);
        parse_int(obj, "\"attack_power\"", t.attack_power);
        if (!parse_int(obj, "\"radius\"", t.radius)) t.radius = 2;  /* default */
        input.towers.push_back(t);
    }

    /* enemies array */
    input.enemies.clear();
    std::string enemies_arr = extract_array(text, "\"enemies\"");
    for (const std::string &obj : split_objects(enemies_arr)) {
        Enemy e{};
        parse_int(obj, "\"id\"",  e.id);
        parse_int(obj, "\"row\"", e.row);
        parse_int(obj, "\"col\"", e.col);
        parse_int(obj, "\"hp\"",  e.hp);
        if (!parse_int(obj, "\"speed\"",   e.speed))   e.speed = 1;    /* default */
        if (!parse_int(obj, "\"path_id\"", e.path_id)) e.path_id = 0;  /* default */
        input.enemies.push_back(e);
    }

    /* paths: an array of arrays of {row, col} (one path per spawn point) */
    input.paths.clear();
    std::string paths_body = extract_balanced_array(text, "\"paths\"");
    for (const std::string &sub : split_arrays(paths_body)) {
        std::vector<Position> path;
        for (const std::string &obj : split_objects(sub)) {
            Position p{};
            parse_int(obj, "\"row\"", p.row);
            parse_int(obj, "\"col\"", p.col);
            path.push_back(p);
        }
        input.paths.push_back(path);
    }

    return true;
}

bool read_input(const std::string &path, GameInput &input) {
    std::string text;
    if (!read_file(path, text)) return false;
    return parse_input(text, input);
}

/* -------------------------------------------------------------------------
 * Public API -- serialising state.json
 * ---------------------------------------------------------------------- */

std::string serialise_state(const GameState &state) {
    std::ostringstream o;
    o << "{\n";
    o << "  \"castle_hp\": " << state.castle_hp << ",\n";

    /* towers_by_power (in-order BST traversal) */
    o << "  \"towers_by_power\": [\n";
    for (std::size_t i = 0; i < state.towers_by_power.size(); i++) {
        const Tower &t = state.towers_by_power[i];
        o << "    {\"id\": " << t.id
          << ", \"row\": " << t.row
          << ", \"col\": " << t.col
          << ", \"attack_power\": " << t.attack_power
          << ", \"radius\": " << t.radius << "}";
        if (i + 1 < state.towers_by_power.size()) o << ",";
        o << "\n";
    }
    o << "  ],\n";

    /* enemies */
    o << "  \"enemies\": [\n";
    for (std::size_t i = 0; i < state.enemies.size(); i++) {
        const Enemy &e = state.enemies[i];
        o << "    {\"id\": " << e.id
          << ", \"row\": " << e.row
          << ", \"col\": " << e.col
          << ", \"hp\": " << e.hp
          << ", \"speed\": " << e.speed
          << ", \"path_id\": " << e.path_id << "}";
        if (i + 1 < state.enemies.size()) o << ",";
        o << "\n";
    }
    o << "  ],\n";

    /* events */
    o << "  \"events\": [\n";
    for (std::size_t i = 0; i < state.events.size(); i++) {
        const Event &ev = state.events[i];
        o << "    {\"type\": \"" << ev.type << "\""
          << ", \"tower_id\": " << ev.tower_id
          << ", \"enemy_id\": " << ev.enemy_id << "}";
        if (i + 1 < state.events.size()) o << ",";
        o << "\n";
    }
    o << "  ]\n";

    o << "}\n";
    return o.str();
}

bool write_state(const std::string &path, const GameState &state) {
    return write_file(path, serialise_state(state));
}
