package bgu.spl.mics.application.messages;

import bgu.spl.mics.Event;

public class GadgetAvailableEvent implements Event<Boolean> {

    private String gadgetName;
    private int timeTick;

    public GadgetAvailableEvent(String gadgetName) {
        this.gadgetName = gadgetName;
    }

    public String getGadgetName() {
        return gadgetName;
    }

    public int getTimeTick() {
        return timeTick;
    }

    public void setTimeTick(int tick) {
        timeTick = tick;
    }
}
