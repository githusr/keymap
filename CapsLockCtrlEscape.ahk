g_LastCtrlKeyDownTime := 0
g_AbortSendEsc := false
g_ControlRepeatDetected := false

*CapsLock::
    if (g_ControlRepeatDetected)
    {
        return
    }

    send,{Ctrl down}
    g_LastCtrlKeyDownTime := A_TickCount
    g_AbortSendEsc := false
    g_ControlRepeatDetected := true

    return

*CapsLock Up::
    send,{Ctrl up}
    g_ControlRepeatDetected := false
    if (g_AbortSendEsc)
    {
        return
    }
    current_time := A_TickCount
    time_elapsed := current_time - g_LastCtrlKeyDownTime
    if (time_elapsed <= 250)
    {
        SendInput {Esc}
    }
    return

~*^a::
    g_AbortSendEsc := true
    return
~*^b::
    g_AbortSendEsc := true
    return
~*^c::
    g_AbortSendEsc := true
    return
~*^d::
    g_AbortSendEsc := true
    return
~*^e::
    g_AbortSendEsc := true
    return
~*^f::
    g_AbortSendEsc := true
    return
~*^g::
    g_AbortSendEsc := true
    return
~*^h::
    g_AbortSendEsc := true
    return
~*^i::
    g_AbortSendEsc := true
    return
~*^j::
    g_AbortSendEsc := true
    return
~*^k::
    g_AbortSendEsc := true
    return
~*^l::
    g_AbortSendEsc := true
    return
~*^m::
    g_AbortSendEsc := true
    return
~*^n::
    g_AbortSendEsc := true
    return
~*^o::
    g_AbortSendEsc := true
    return
~*^p::
    g_AbortSendEsc := true
    return
~*^q::
    g_AbortSendEsc := true
    return
~*^r::
    g_AbortSendEsc := true
    return
~*^s::
    g_AbortSendEsc := true
    return
~*^t::
    g_AbortSendEsc := true
    return
~*^u::
    g_AbortSendEsc := true
    return
~*^v::
    g_AbortSendEsc := true
    return
~*^w::
    g_AbortSendEsc := true
    return
~*^x::
    g_AbortSendEsc := true
    return
~*^y::
    g_AbortSendEsc := true
    return
~*^z::
    g_AbortSendEsc := true
    return
