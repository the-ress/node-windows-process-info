#define _WIN32_WINNT 0x0600

#include <nan.h>
#include <TlHelp32.h>
#include <windows.h>
#include <unordered_set>

__time64_t FileTimeToUnixTime(FILETIME & ft)
{
  ULARGE_INTEGER ull;

  ull.LowPart = ft.dwLowDateTime;
  ull.HighPart = ft.dwHighDateTime;

  return ull.QuadPart / 10000000ULL - 11644473600ULL;
}

static NAN_METHOD(GetProcessInfo) {
  Nan::HandleScope scope;

  if (info.Length() != 1
    || !info[0]->IsArray()) // pid array
  {
    Nan::ThrowError("Usage: pty.getConsoleProcessList(processList)");
    return;
  }

  auto pidsArray = v8::Local<v8::Array>::Cast(info[0]);
  auto pids = std::vector<int>(pidsArray->Length());

  for (unsigned int i = 0; i < pidsArray->Length(); i++) {
    auto value = Nan::Get(pidsArray, i).ToLocalChecked();
    if (!value->IsInt32())
    {
      Nan::ThrowError("Usage: pty.getConsoleProcessList(processList)");
      return;
    }
    pids[i] = value->Int32Value();
  }

  auto pidLookup = std::unordered_set<int>(pids.begin(), pids.end());

  auto result = Nan::New<v8::Array>();
  auto i = 0;

  auto hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

  if (hSnapshot == INVALID_HANDLE_VALUE)
  {
    //GetLastError();
    Nan::ThrowError("");
    return;
  }

  PROCESSENTRY32 entry;
  entry.dwSize = sizeof(entry);

  Process32First(hSnapshot, &entry);
  do
  {
    auto pid = entry.th32ProcessID;

    if (pidLookup.find(pid) == pidLookup.end()) continue;

    auto hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

    auto processResult = Nan::New<v8::Object>();

    processResult->Set(Nan::New<v8::String>("processId").ToLocalChecked(), Nan::New<v8::Number>(pid));
    processResult->Set(Nan::New<v8::String>("name").ToLocalChecked(), Nan::New<v8::String>(entry.szExeFile).ToLocalChecked());
    processResult->Set(Nan::New<v8::String>("parentId").ToLocalChecked(), Nan::New<v8::Number>(entry.th32ParentProcessID));

    FILETIME fProcessTime, ftExit, ftKernel, ftUser;
    if (GetProcessTimes(hProc, &fProcessTime, &ftExit, &ftKernel, &ftUser)) {
      auto unixTime = FileTimeToUnixTime(fProcessTime) * 1000;
      processResult->Set(Nan::New<v8::String>("startTime").ToLocalChecked(), Nan::New<v8::Date>(unixTime).ToLocalChecked());
    }

    DWORD value = MAX_PATH;
    char buffer[MAX_PATH];
    if (QueryFullProcessImageName(hProc, 0, buffer, &value))
    {
      processResult->Set(Nan::New<v8::String>("path").ToLocalChecked(), Nan::New<v8::String>(buffer).ToLocalChecked());
    }

    result->Set(i, processResult);
    i++;
  } while (Process32Next(hSnapshot, &entry));

  return info.GetReturnValue().Set(result);
}

void init(v8::Handle<v8::Object> target) {
  Nan::HandleScope scope;
  Nan::SetMethod(target, "getProcessInfo", GetProcessInfo);
}

NODE_MODULE(process_info, init);
