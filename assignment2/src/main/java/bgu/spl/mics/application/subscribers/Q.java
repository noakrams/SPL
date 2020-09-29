package bgu.spl.mics.application.subscribers;

import bgu.spl.mics.Subscriber;
import bgu.spl.mics.application.messages.GadgetAvailableEvent;
import bgu.spl.mics.application.messages.TickBroadcast;
import bgu.spl.mics.application.passiveObjects.Inventory;
import java.util.concurrent.CountDownLatch;

/**
 * Q is the only Subscriber\Publisher that has access to the {@link bgu.spl.mics.application.passiveObjects.Inventory}.
 * <p>
 * You can add private fields and public methods to this class.
 * You MAY change constructor signatures and even add new public constructors.
 */
public class Q extends Subscriber {

    private CountDownLatch latch;
    int tick;

    public Q(CountDownLatch latch) {
        super("Q");
        this.latch = latch;
    }

    @Override
    protected void initialize() {
        subscribeBroadcast(TickBroadcast.class, tickBroadcast -> {
            tick = tickBroadcast.getCurrTick();
            if (tickBroadcast.getDuration() == tick) {
                terminate();
            }
        });
        subscribeEvent(GadgetAvailableEvent.class, GadgetAvailableEvent -> {
            //Take the gadget from the inventory if exist
            boolean isGadgetAvailable = Inventory.getInstance().getItem(GadgetAvailableEvent.getGadgetName());
            GadgetAvailableEvent.setTimeTick(tick);
            complete(GadgetAvailableEvent, isGadgetAvailable);
        });
        latch.countDown();
    }

}
