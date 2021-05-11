#pragma once

#include <map>


template<class KeyType>
class FileCursoresController;

namespace
{
  template<class KeyType, class Callback>
  class FileCursor
  {
    using MainClass = typename FileCursor<KeyType, Callback>;
    Ptr(MainClass)

  public:
    friend FileCursoresController<KeyType>;

  public:
    struct Position
    {
      FileContentChangerI::FileSizeType start;
      FileContentChangerI::FileSizeType offset;
      FileContentChangerI::FileSizeType maximum;
    };

  private:
    FileContentChangerI::ShaPtr _fileContentChanger;
    Position _position;
    Callback _callback;

  private:
    FileCursor(FileContentChangerI::ShaPtr fileContentChanger, Position position, Callback callback)
      : _fileContentChanger(std::move(fileContentChanger))
      , _position{ position }
      , _callback{ callback }
    {
    }

  public:
    bool operator < (MainClass lrh)
    {
      return this->_position.maximum < lrh._position.maximum;
    }

    void SetCursorToEnd()
    {
      _fileContentChanger->SetCursorPosition(_position.start + _position.maximum);
      _position.offset = _position.maximum;
    }

    void SetCursorToBegin()
    {
      _fileContentChanger->SetCursorPosition(_position.start);
      _position.offset = 0;
    }

    void SetCursorPosition(FileContentChangerI::FileSizeType position)
    {
      if (position > _position.maximum)
      {
        // need exception there
        //FileContentChangerI::Errors.push(std::make_unique<FileContentChangerI::ErrorMain>(ErrorOutOfCursorAreaSize()));
      }
      else
      {
        _fileContentChanger->SetCursorPosition(_position.start + position);
        _position.offset = position;
      }
    }

    FileContentChangerI::FileSizeType GetCursorPosition()
    {
      return _position.offset;
    }

    void WriteData(const FileContentChangerI::BufferType* data, FileContentChangerI::FileSizeType size)
    {
      _fileContentChanger->WriteData(data, size);
      _position.offset += size;
      _position.maximum += size;
      _callback();
    }

    std::vector<FileContentChangerI::BufferType> ReadData(FileContentChangerI::FileSizeType size)
    {
      if (size > (_position.maximum - _position.offset))
      {
        // need exception there
        //FileContentChangerI::Errors.push(std::make_unique<FileContentChangerI::ErrorMain>(ErrorOutOfCursorAreaSize()));
      }
      else
      {
        _position.offset += size;
        return _fileContentChanger->ReadData(size);
      }
    }
  };
}

template<class KeyType>
class FileCursoresController
{
  using MainClass = typename FileCursoresController<KeyType>;
  Ptr(MainClass)

private:
  class CursorClassCallback
  {
  private:
    MainClass::Ptr _fcc;

  public:
    CursorClassCallback(MainClass::Ptr fcc)
      : _fcc(fcc)
    {
    }

    void operator()()
    {
      _fcc->Check();
      return;
    }
  };

public:
  using CursorClass = typename FileCursor<KeyType, CursorClassCallback>;
  using CursorClassCompare = typename std::function<bool(typename CursorClass::WekPtr, typename CursorClass::WekPtr)>;

private:
  FileContentChangerI::ShaPtr _fileContentChanger;
  std::map<KeyType, typename CursorClass::ShaPtr> _cursoresMap;
  std::set<typename CursorClass::WekPtr, CursorClassCompare> _cursoresSet;

private:
  void Check()
  {
    if (_cursoresSet.size() < 2)
      return;
    for (auto _1 = _cursoresSet.begin(), _2 = ++_cursoresSet.begin(); _2 != _cursoresSet.end() ; ++_1, ++_2)
    {
      auto _2_real_start = _1->lock()->_position.start + _1->lock()->_position.maximum;
      if (_2_real_start > _2->lock()->_position.start)
        _2->lock()->_position.start = _2_real_start;
    }
  }

public:
  FileCursoresController(FileContentChangerI::ShaPtr fileContentChanger)
    : _fileContentChanger(std::move(fileContentChanger))
    , _cursoresMap{}
    , _cursoresSet{ [](typename CursorClass::WekPtr l, typename CursorClass::WekPtr r) -> bool { return *(l.lock()) < *(r.lock()); } }
  {
  }

  typename CursorClass::WekPtr AddCursore(KeyType key, typename CursorClass::Position position)
  {
    auto cursoreIt = _cursoresMap.find(key);
    if (cursoreIt == _cursoresMap.end())
    {
      CursorClass::Ptr ccPtr = new CursorClass(_fileContentChanger, position, CursorClassCallback{ this });
      std::shared_ptr<CursorClass> ccShaPtr{ ccPtr };
      _cursoresMap.emplace(std::make_pair(std::move(key), ccShaPtr));
      _cursoresSet.emplace(CursorClass::WekPtr{ ccShaPtr });
      cursoreIt = _cursoresMap.find(key);
    }
    return (*cursoreIt).second;
  }

  typename CursorClass::WekPtr GetCursore(const KeyType& key)
  {
    std::weak_ptr<FileCursor> result;
    auto cursoreIt = _cursoresMap.find(key);
    if (cursoreIt != _cursoresMap.end())
      result = *cursoreIt.second;
    return result;
  }

  size_t GetCursoresQuantity()
  {
    return _cursoresMap.size();
  }
};
