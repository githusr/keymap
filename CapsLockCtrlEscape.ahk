#Requires AutoHotkey 2.0.19 64-bit
#SingleInstance Force

SetStoreCapsLockMode false
LShift & RShift:: SetCapsLockState !GetKeyState("CapsLock", "T")

*CapsLock:: {
    Send "{LCtrl down}"
    LastCtrlKeyDownTime := A_TickCount
    KeyWait "CapsLock"
    Send "{LCtrl up}"
    if A_PriorKey = "CapsLock" && A_TickCount - LastCtrlKeyDownTime <= 250 {
        Send "{Esc}"
    }
}
