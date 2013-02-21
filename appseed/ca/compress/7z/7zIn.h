// 7zIn.h
// from 7-zip on dawn of 13/01/2001 - Thursday
#pragma once

namespace n7z
{

   struct CInArchiveInfo
   {
      CArchiveVersion Version;
      file_position StartPosition;
      file_position StartPositionAfterHeader;
      file_position DataStartPosition;
      file_position DataStartPosition2;
      base_array<uint64_t> FileInfoPopIDs;
      void clear()
      {
         FileInfoPopIDs.remove_all();
      }
   };

   struct CArchiveDatabaseEx: public CArchiveDatabase
   {
      CInArchiveInfo ArchiveInfo;
      base_array<file_position> PackStreamStartPositions;
      base_array<CNum> FolderStartPackStreamIndex;
      base_array<CNum> FolderStartFileIndex;
      base_array<CNum> FileIndexToFolderIndexMap;

      file_size HeadersSize;
      file_size PhySize;

      void clear()
      {
         CArchiveDatabase::clear();
         ArchiveInfo.clear();
         PackStreamStartPositions.remove_all();
         FolderStartPackStreamIndex.remove_all();
         FolderStartFileIndex.remove_all();
         FileIndexToFolderIndexMap.remove_all();

         HeadersSize = 0;
         PhySize = 0;
      }

      void FillFolderStartPackStream();
      void FillStartPos();
      void FillFolderStartFileIndex();

      void Fill()
      {
         FillFolderStartPackStream();
         FillStartPos();
         FillFolderStartFileIndex();
      }

      file_position GetFolderStreamPos(int32_t folderIndex, int32_t indexInFolder) const
      {
         return ArchiveInfo.DataStartPosition +
            PackStreamStartPositions[FolderStartPackStreamIndex[folderIndex] + indexInFolder];
      }

      file_size GetFolderFullPackSize(int32_t folderIndex) const
      {
         CNum packStreamIndex = FolderStartPackStreamIndex[folderIndex];
         const CFolder &folder = Folders[folderIndex];
         file_size size = 0;
         for (int32_t i = 0; i < folder.PackStreams.get_count(); i++)
            size += PackSizes[packStreamIndex + i];
         return size;
      }

      file_size GetFolderPackStreamSize(int32_t folderIndex, int32_t streamIndex) const
      {
         return PackSizes[FolderStartPackStreamIndex[folderIndex] + streamIndex];
      }

      file_size GetFilePackSize(CNum fileIndex) const
      {
         CNum folderIndex = FileIndexToFolderIndexMap[fileIndex];
         if (folderIndex != kNumNoIndex)
            if (FolderStartFileIndex[folderIndex] == fileIndex)
               return GetFolderFullPackSize(folderIndex);
         return 0;
      }
   };

   class CInByte2
   {
      const byte *_buffer;
      size_t _size;
   public:
      size_t _pos;
      void Init(const byte *buffer, size_t size)
      {
         _buffer = buffer;
         _size = size;
         _pos = 0;
      }
      byte ReadByte();
      void ReadBytes(byte *data, size_t size);
      void SkipData(file_size size);
      void SkipData();
      uint64_t ReadNumber();
      CNum ReadNum();
      uint32_t ReadUInt32();
      uint64_t ReadUInt64();
      void ReadString(string &s);
   };

   class CStreamSwitch;

   const uint32_t kHeaderSize = 32;

   class CInArchive :
      virtual ::gen::object
   {
      friend class CStreamSwitch;

      ::ca::smart_pointer < ::gen::byte_input_stream > _stream;

      array_ptr_alloc < CInByte2 > _inByteVector;
      CInByte2 *_inByteBack;

      file_position _arhiveBeginStreamPosition;

      byte _header[kHeaderSize];

      file_size HeadersSize;

      void AddByteStream(const byte *buffer, size_t size)
      {
         _inByteVector.add(CInByte2());
         _inByteBack = &_inByteVector.last_element();
         _inByteBack->Init(buffer, size);
      }

      void DeleteByteStream()
      {
         _inByteVector.remove_last();
         if (!_inByteVector.is_empty())
            _inByteBack = &_inByteVector.last_element();
      }

   private:
      HRESULT FindAndReadSignature(::gen::byte_input_stream *stream, const file_position *searchHeaderSizeLimit);

      void ReadBytes(byte *data, size_t size) { _inByteBack->ReadBytes(data, size); }
      byte ReadByte() { return _inByteBack->ReadByte(); }
      uint64_t ReadNumber() { return _inByteBack->ReadNumber(); }
      CNum ReadNum() { return _inByteBack->ReadNum(); }
      uint64_t ReadID() { return _inByteBack->ReadNumber(); }
      uint32_t ReadUInt32() { return _inByteBack->ReadUInt32(); }
      uint64_t ReadUInt64() { return _inByteBack->ReadUInt64(); }
      void SkipData(file_size size) { _inByteBack->SkipData(size); }
      void SkipData() { _inByteBack->SkipData(); }
      void WaitAttribute(uint64_t attribute);

      void ReadArchiveProperties(CInArchiveInfo &archiveInfo);
      void GetNextFolderItem(CFolder &itemInfo);
      void ReadHashDigests(int32_t numItems,
         bool_array &digestsDefined, base_array<uint32_t> &digests);

      void ReadPackInfo(
         file_position &dataOffset,
         base_array<file_size> &packSizes,
         bool_array &packCRCsDefined,
         base_array<uint32_t> &packCRCs);

      void ReadUnpackInfo(
         const array_ptr_alloc < ::gen::byte_buffer >  *dataVector,
         array_ptr_alloc < CFolder > &folders);

      void ReadSubStreamsInfo(
         const array_ptr_alloc<CFolder> &folders,
         base_array<CNum> &numUnpackStreamsInFolders,
         base_array<file_size> &unpackSizes,
         bool_array &digestsDefined,
         base_array<uint32_t> &digests);

      void ReadStreamsInfo(
         const array_ptr_alloc < ::gen::byte_buffer >  *dataVector,
         file_position &dataOffset,
         base_array<file_size> &packSizes,
         bool_array &packCRCsDefined,
         base_array<uint32_t> &packCRCs,
         array_ptr_alloc<CFolder> &folders,
         base_array<CNum> &numUnpackStreamsInFolders,
         base_array<file_size> &unpackSizes,
         bool_array &digestsDefined,
         base_array<uint32_t> &digests);


      void ReadBoolVector(int32_t numItems, bool_array &v);
      void ReadBoolVector2(int32_t numItems, bool_array &v);
      void ReadUInt64DefVector(const array_ptr_alloc < ::gen::byte_buffer > &dataVector, CUInt64DefVector &v, int32_t numFiles);
      HRESULT ReadAndDecodePackedStreams(
         ::libcompress::codecs_info_interface *codecsInfo, const base_array < ::libcompress::codec_info_ex > *externalCodecs,
         file_position baseOffset, file_position &dataOffset,
         array_ptr_alloc < ::gen::byte_buffer > &dataVector,
         ::crypto::get_text_password_interface *getTextPassword, bool &passwordIsDefined
         );
      HRESULT ReadHeader(
         ::libcompress::codecs_info_interface *codecsInfo, const base_array < ::libcompress::codec_info_ex > *externalCodecs,
         CArchiveDatabaseEx &db,
         ::crypto::get_text_password_interface *getTextPassword, bool &passwordIsDefined
         );
      HRESULT ReadDatabase2(
         ::libcompress::codecs_info_interface *codecsInfo, const base_array < ::libcompress::codec_info_ex > *externalCodecs,
         CArchiveDatabaseEx &db,
         ::crypto::get_text_password_interface *getTextPassword, bool &passwordIsDefined
         );
   public:

      CInArchive(::ca::application * papp);
      virtual ~CInArchive();

      HRESULT Open(::gen::byte_input_stream *stream, const file_position *searchHeaderSizeLimit); // S_FALSE means is not archive
      void Close();

      HRESULT ReadDatabase(
         ::libcompress::codecs_info_interface *codecsInfo, const base_array < ::libcompress::codec_info_ex > *externalCodecs,
         CArchiveDatabaseEx &db,
         ::crypto::get_text_password_interface *getTextPassword, bool &passwordIsDefined
         );
   };

} // namespace n7z
