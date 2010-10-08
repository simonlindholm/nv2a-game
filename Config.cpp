#include <SDL/SDL.h>
#include <fstream>
#include <vector>
#include <utility>
#include <climits>
#include "Config.h"
#include "exceptions.h"
#include "shared_ptr.h"
using namespace std;

struct UserConfig {
	shared_ptr<int> fps;
	shared_ptr<pair<int, int> > dim;
	shared_ptr<bool> fullscreen;
};

// Read values from a user configuration (which can include unset values)
// into an actual configuration.
static void setFromUserConfig(Config& config, UserConfig& ucfg) {

	// Set fullscreen flag
	config.fullscreen = (ucfg.fullscreen ? *ucfg.fullscreen : false);

	// Set window dimensions
	if (ucfg.dim) {
		config.winWidth = ucfg.dim->first;
		config.winHeight = ucfg.dim->second;
	}
	else {
		if (config.fullscreen) {
			const SDL_VideoInfo* info = SDL_GetVideoInfo();
			config.winWidth = info->current_w;
			config.winHeight = info->current_h;
		}
		else {
			config.winWidth = 640;
			config.winHeight = 480;
		}
	}

	// Set minimum frame delay from FPS
	int fps = (ucfg.fps ? *ucfg.fps : 60);
	if (fps <= 0) {
		config.minDelay = 0;
	}
	else {
		config.minDelay = 1000 / fps;
	}
}

Config::Config() {
	// Initalize configuration with default values by setting it from
	// an empty user configuration
	UserConfig ucfg;
	setFromUserConfig(*this, ucfg);
}

// Trim whitespace from the beginning and the end of a string
static string trimWhitespace(const string& str) {
	size_t start = 0;
	while (start < str.size() && isspace(str[start])) ++start;
	if (start == str.size()) return "";

	size_t end = str.size() - 1;
	while (end > start && isspace(str[end])) --end;

	return str.substr(start, end - start + 1);
}

// Parse the end a config string
static string parseLast(const string& sec) {
	string value;
	if (sec.size() == 0) value = "";
	else if (sec[0] == '"') {
		size_t pos = 1;
		for(; pos < sec.size(); ++pos) {
			char c = sec[pos];
			if (c == '\\') {
				if (++pos == sec.size())
					throw ConfigException("Invalid syntax (trailing backslash).");
				c = sec[pos];
				if (c == '"') c = '"';
				else if (c == '\\') c = '\\';
				else if (c == 'n') c = '\n';
				else if (c == 't') c = '\t';
				else if (c == 'r') c = '\r';
				else{
					throw ConfigException("Invalid syntax (unknown escape character).");
				}
				value += c;
			}
			else if(c == '"') {
				break;
			}
			else {
				value += c;
			}
		}
		if (pos == sec.size())
			throw ConfigException("Invalid syntax (end of quote expected).");
		++pos;
		size_t fcom = sec.find('#', pos);
		string extra = (fcom == string::npos ? sec.substr(pos) :
						sec.substr(pos, fcom - pos));
		if (trimWhitespace(extra) != "")
			throw ConfigException("Invalid syntax (several values provided).");
	}
	else {
		size_t fcom = sec.find('#');
		value = trimWhitespace(sec.substr(0, fcom));
	}
	return value;
}

static shared_ptr<bool> parseBoolean(const string& str) {
	if (str == "") return shared_ptr<bool>();
	if (str == "true") return shared_ptr<bool>(new bool(true));
	if (str == "false") return shared_ptr<bool>(new bool(false));
	throw ConfigException("Invalid parameter (must be a boolean).");
}

static int parseUintRaw(const string& str) {
	if (str == "") return -1;
	int val = 0;
	for (size_t i = 0; i < str.size(); ++i) {
		char c = str[i];
		if ('0' <= c && c <= '9') {
			val *= 10;
			val += (c - '0');
		}
		else {
			return -1;
		}
	}
	return val;
}

static shared_ptr<int> parseInt(const string& str) {
	if (str == "") return shared_ptr<int>();
	string s = str;
	bool neg = false;
	if (s[0] == '-') {
		neg = true;
		s = s.substr(1);
	}
	int val = parseUintRaw(s);
	if (val == -1) {
		throw ConfigException("Invalid parameter (must be an integer).");
	}
	if (neg) val = -val;
	return shared_ptr<int>(new int(val));
}

static shared_ptr<pair<int, int> > parseDimension(const string& str) {
	if (str == "") return shared_ptr<pair<int, int> >();
	size_t p = str.find('x');
	if (p == string::npos) {
		throw ConfigException("Invalid parameter (must be in format WxH).");
	}

	int w = parseUintRaw(str.substr(0, p));
	int h = parseUintRaw(str.substr(p + 1));
	if (w == -1 || h == -1) {
		throw ConfigException("Invalid parameter (must be in format WxH).");
	}

	return shared_ptr<pair<int, int> >(new pair<int, int>(w, h));
}

void Config::load(const char* filename) {
	vector<pair<string, string> > properties;

	ifstream fin(filename, ios_base::binary | ios_base::in);
	string line;
	while(getline(fin, line)) {
		size_t eq = line.find('=');

		// Check that the first part is valid, by checking that it is not the
		// only part, and that it does not contain line-ending comments
		string prop = trimWhitespace(line.substr(0, eq));

		size_t fcom = prop.find('#');
		if (fcom != string::npos) {
			// If the line begins with a comment, it is empty, so continue with
			// the next one.
			if (fcom == 0) continue;

			// Otherwise, the line has contents, so if we find a '#' before
			// the '=' character, the syntax is invalid.
			throw ConfigException("Invalid syntax (missing value).");
		}

		if (eq == string::npos) {
			// There is no '=' character, so if the first part is empty, so is
			// the whole line, so continue with next one.
			if (prop.size() == 0) continue;

			// Otherwise, the non-empty line has no '=' character, so the
			// syntax is invalid.
			throw ConfigException("Invalid syntax (missing value).");
		}

		// Parse the second part
		string sec = trimWhitespace(line.substr(eq + 1));
		string value = parseLast(sec);

		properties.push_back(make_pair(prop, value));
	}

	if (!properties.empty()) {
		UserConfig ucfg;

		for (size_t i = 0; i < properties.size(); ++i) {
			const pair<string, string>& prop = properties[i];
			const string& p = prop.first;
			const string& v = prop.second;

			if (p == "fullscreen") {
				ucfg.fullscreen = parseBoolean(v);
			}
			else if (p == "dimensions") {
				ucfg.dim = parseDimension(v);
			}
			else if (p == "fps") {
				ucfg.fps = parseInt(v);
			}
			else {
				throw ConfigException("Invalid option.");
			}
		}

		setFromUserConfig(*this, ucfg);
	}
}

Config& Config::get() {
	static Config singleton;
	return singleton;
}
