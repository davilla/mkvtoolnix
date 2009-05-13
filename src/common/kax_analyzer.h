/*
   mkvmerge -- utility for splicing together matroska files
   from component media subtypes

   Distributed under the GPL
   see the file COPYING for details
   or visit http://www.gnu.org/copyleft/gpl.html

   quick Matroska file parsing

   Written by Moritz Bunkus <moritz@bunkus.org>.
*/

#ifndef __MTX_COMMON_KAX_ANALYZER_H
#define __MTX_COMMON_KAX_ANALYZER_H

#include "os.h"

#include <map>

#include <matroska/KaxSegment.h>

#include "common/matroska.h"
#include "common/common.h"
#include "common/mm_io.h"

using namespace libebml;
using namespace libmatroska;

class kax_analyzer_data_c;
typedef counted_ptr<kax_analyzer_data_c> kax_analyzer_data_cptr;

class kax_analyzer_data_c {
public:
  EbmlId m_id;
  int64_t m_pos, m_size;

public:
  static kax_analyzer_data_cptr create(const EbmlId id, int64_t pos, int64_t size) {
    return kax_analyzer_data_cptr(new kax_analyzer_data_c(id, pos, size));
  }

public:
  kax_analyzer_data_c(const EbmlId id, int64_t pos, int64_t size)
    : m_id(id)
    , m_pos(pos)
    , m_size(size)
  {
  }
};

bool operator <(const kax_analyzer_data_cptr &d1, const kax_analyzer_data_cptr &d2);

class kax_analyzer_c {
public:
  enum update_element_result_e {
    uer_success,
    uer_error_segment_size_for_element,
    uer_error_segment_size_for_meta_seek,
    uer_error_meta_seek,
    uer_error_not_indexable,
    uer_error_unknown,
  };

public:
  std::vector<kax_analyzer_data_cptr> m_data;

private:
  std::string m_file_name;
  mm_file_io_c *m_file;
  bool m_close_file;
  counted_ptr<KaxSegment> m_segment;
  std::map<int64_t, bool> m_meta_seeks_by_position;
  EbmlStream *m_stream;

public:                         // Static functions
  static bool probe(std::string file_name);

public:
  kax_analyzer_c(std::string file_name);
  kax_analyzer_c(mm_file_io_c *file);
  virtual ~kax_analyzer_c();

  virtual update_element_result_e update_element(EbmlElement *e, bool write_defaults = false);
  virtual EbmlMaster *read_all(const EbmlCallbacks &callbacks);

  virtual EbmlElement *read_element(kax_analyzer_data_c *element_data);
  virtual EbmlElement *read_element(kax_analyzer_data_cptr element_data) {
    return read_element(element_data.get());
  }
  virtual EbmlElement *read_element(unsigned int pos) {
    return read_element(m_data[pos]);
  }

  virtual int find(const EbmlId &id) {
    unsigned int i;

    for (i = 0; m_data.size() > i; i++)
      if (id == m_data[i]->m_id)
        return i;

    return -1;
  }

  virtual bool process(bool parse_fully = true);

  virtual void show_progress_start(int64_t size) {
  }
  virtual bool show_progress_running(int percentage) {
    return true;
  }
  virtual void show_progress_done() {
  }

protected:
  virtual void remove_from_meta_seeks(EbmlId id);
  virtual void overwrite_all_instances(EbmlId id);
  virtual void merge_void_elements();
  virtual void write_element(EbmlElement *e, bool write_defaults);
  virtual void add_to_meta_seek(EbmlElement *e);

  virtual void adjust_segment_size();
  virtual bool create_void_element(int64_t file_pos, int void_size, int data_idx, bool add_new_data_element);

  virtual void debug_dump_elements();

  virtual void read_all_meta_seeks();
  virtual void read_meta_seek(int64_t pos);
};
typedef counted_ptr<kax_analyzer_c> kax_analyzer_cptr;

#endif  // __MTX_COMMON_KAX_ANALYZER_H