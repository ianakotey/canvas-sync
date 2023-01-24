#ifndef SERVER_H
#define SERVER_H

#include "canvas_api.h"
#include "filetree.h"
#include "httpjson.h"

namespace canvas
{
class Server
{
private:
  CanvasApi *api;
  FileTree tree;
  vector<Course> courses;
  string base_url;

public:
  Server(const char *token, const char *base_url);
  Server(string *token, string *base_url);
  void run();
  void run_debug();

  void load_tree();
  string dump_tree();
  FileTree *get_tree();

  bool valid_token();

  vector<File> folder_files(const int *folder_id);

  void set_token(string *token);
};
}; // namespace canvas

#endif
