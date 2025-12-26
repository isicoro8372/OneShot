#ifndef KEYLOGGER_H_
#define KEYLOGGER_H_

#define MAX_INPUT (256)

void StoreKeyLog(UINT uMsg, WPARAM keys);
void UpdateKeyLog();
bool GetKeyTrigger(WPARAM keyType);
bool GetKeyUp(WPARAM keyType);
bool GetKeyDown(WPARAM keyType, unsigned int frame = 1);

#endif // !KEYLOGGER_H_