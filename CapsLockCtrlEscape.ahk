#Requires AutoHotkey 2.0.19 64-bit
#SingleInstance Force

SetStoreCapsLockMode false
LShift & RShift:: SetCapsLockState !GetKeyState("CapsLock", "T")

g_AbortSendEsc := false

*CapsLock:: {
    global g_AbortSendEsc := false
    Send "{LCtrl down}"
    LastCtrlKeyDownTime := A_TickCount
    KeyWait "CapsLock"
    Send "{LCtrl up}"
    if (!g_AbortSendEsc && A_TickCount - LastCtrlKeyDownTime <= 250) {
        Send "{Esc}"
    }
}

~*<^a::
~*<^b::
~*<^c::
~*<^d::
~*<^e::
~*<^f::
~*<^g::
~*<^h::
~*<^i::
~*<^j::
~*<^k::
~*<^l::
~*<^m::
~*<^n::
~*<^o::
~*<^p::
~*<^q::
~*<^r::
~*<^s::
~*<^t::
~*<^u::
~*<^v::
~*<^w::
~*<^x::
~*<^y::
~*<^z::
~*<^1::
~*<^2::
~*<^3::
~*<^4::
~*<^5::
~*<^6::
~*<^7::
~*<^8::
~*<^9::
~*<^0::
~*<^Space::
~*<^Backspace::
~*<^Delete::
~*<^Insert::
~*<^Home::
~*<^End::
~*<^PgUp::
~*<^PgDn::
~*<^Tab::
~*<^Enter::
~*<^,::
~*<^.::
~*<^/::
~*<^;::
~*<^'::
~*<^[::
~*<^]::
~*<^\::
~*<^-::
~*<^=::
~*<^`::
~*<^F1::
~*<^F2::
~*<^F3::
~*<^F4::
~*<^F5::
~*<^F6::
~*<^F7::
~*<^F8::
~*<^F9::
~*<^F10::
~*<^F11::
~*<^F12:: {
    global g_AbortSendEsc := true
}
