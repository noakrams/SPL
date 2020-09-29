package bgu.spl.net.Frames.In;

import bgu.spl.net.Frames.Frame;
import bgu.spl.net.impl.stomp.StompMessagingProtocolImpl;

public abstract class FrameIn extends Frame {
    public abstract void execute(StompMessagingProtocolImpl protocol);
}