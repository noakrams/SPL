package bgu.spl.mics.application.subscribers;

import bgu.spl.mics.Callback;

import bgu.spl.mics.Subscriber;
import bgu.spl.mics.application.messages.AgentsAvailableEvent;
import bgu.spl.mics.application.messages.MissionReceivedEvent;
import bgu.spl.mics.application.messages.TickBroadcast;
import bgu.spl.mics.application.passiveObjects.MissionInfo;

import java.util.ArrayList;
import java.util.concurrent.CountDownLatch;

/**
 * A Publisher\Subscriber.
 * Holds a list of Info objects and sends them
 * <p>
 * You can add private fields and public methods to this class.
 * You MAY change constructor signatures and even add new public constructors.
 */
public class Intelligence extends Subscriber {

    private int tick;
    private ArrayList<MissionInfo> missionArray;
    private CountDownLatch latch;


    public Intelligence(ArrayList<MissionInfo> missionInfos, String serialNum, CountDownLatch latch) {
        super("Intelligence"+serialNum);
        this.missionArray = missionInfos;
        this.latch = latch;
    }


    @Override
    protected void initialize() {
        subscribeBroadcast(TickBroadcast.class, tickBroadcast -> { //callBack broadcast
            tick = tickBroadcast.getCurrTick();
            if (tickBroadcast.getDuration() == tick) {
                terminate();
            }
            //when orderSchedule list not empty and tick>=first in list
            if (!missionArray.isEmpty() && tick == missionArray.get(0).getTimeIssued()) {
                MissionInfo nextMission = missionArray.remove(0);
                MissionReceivedEvent missionReceivedEvent = new MissionReceivedEvent(nextMission);
                getSimplePublisher().sendEvent(missionReceivedEvent);
            }
        });
        latch.countDown();
    }

}
