#include "string_buffer.h"

#include <QList>

#pragma once

class strings
{
    public:
    strings(QStringView chunk_prefix, size_t bufferSize = STRING_BUFFER_SIZE);
    ~strings();
    void clear();
    void append(QStringView value);
    size_t chunksCount() const;
    string_buffer* getChunk(const size_t index) const;
    void freeChunk(const size_t index);

private:
  QList<string_buffer *> m_chunks;
  const size_t m_bufferSize;
  const QString m_chunk_prefix;

  string_buffer* _newChunk();
};
