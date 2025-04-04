#Requires AutoHotkey 2.0.19 64-bit
#SingleInstance Force

ProcessSetPriority "High"

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

<!q::!F4
<!b::^b
<!f::^f
<!h::^h
<!n::^n
<!p::^p

*<^b:: Send "{Blind^}{Left}"
*<^f:: Send "{Blind^}{Right}"
*<^h:: Send "{Blind^}{BS}"
*<^n:: Send "{Blind^}{Down}"
*<^p:: Send "{Blind^}{Up}"

#HotIf WinActive("ahk_exe msedge.exe")
<^q::^Tab
<^Tab::^q
#HotIf

#e:: {
    if WinExist(" - File Explorer") {
        WinActivate
    } else {
        Run "explorer"
    }
}
