#ifndef MTX_MKVTOOLNIX_GUI_SOURCE_FILE_H
#define MTX_MKVTOOLNIX_GUI_SOURCE_FILE_H

#include "common/common_pch.h"

#include "common/file_types.h"
#include "common/qt.h"
#include "mkvtoolnix-gui/mux_config.h"
#include "mkvtoolnix-gui/track.h"

#include <QHash>
#include <QList>
#include <QObject>
#include <QSettings>
#include <QString>

class SourceFile;
typedef std::shared_ptr<SourceFile> SourceFilePtr;

class SourceFile: public QObject {
  Q_OBJECT;
public:
  QHash<QString, QString> m_properties;
  QString m_fileName, m_container;
  QList<TrackPtr> m_tracks;
  QList<SourceFilePtr> m_additionalParts, m_appendedFiles;

  file_type_e m_type;
  bool m_appended, m_additionalPart;
  SourceFile *m_appendedTo;

public:
  explicit SourceFile(QString const &fileName = QString{""});
  virtual ~SourceFile();

  virtual void setContainer(QString const &container);
  virtual bool isValid() const;
  virtual bool isRegular() const;
  virtual bool isAppended() const;
  virtual bool isAdditionalPart() const;
  virtual bool hasRegularTrack() const;

  virtual void saveSettings(QSettings &settings) const;
  virtual void loadSettings(MuxConfig::Loader &l);
  virtual void fixAssociations(MuxConfig::Loader &l);
};

#endif  // MTX_MKVTOOLNIX_GUI_SOURCE_FILE_H
