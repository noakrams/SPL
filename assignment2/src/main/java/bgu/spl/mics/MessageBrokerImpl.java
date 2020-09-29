package bgu.spl.mics;

import java.util.concurrent.BlockingQueue;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.LinkedBlockingQueue;

/**
 * The {@link MessageBrokerImpl class is the implementation of the MessageBroker interface.
 * Write your implementation here!
 * Only private fields and methods can be added to this class.
 */
public class MessageBrokerImpl implements MessageBroker {

    //subscribes subscriber to message.
    private ConcurrentHashMap<Class<? extends Message>, ConcurrentLinkedQueue<Subscriber>> subscriberByMessage;
    //queue for each subscriber - round robin.
    private ConcurrentHashMap<Subscriber, BlockingQueue<Message>> messagePerSubscriber;
    //need to check about null message.
    private ConcurrentHashMap<Message, Future> messageFuture;

    //singelton
    private static class MessageBrokerHolder {
        private static MessageBrokerImpl instance = new MessageBrokerImpl();
    }

    //constructor
    private MessageBrokerImpl() {
        subscriberByMessage = new ConcurrentHashMap<>();
        messagePerSubscriber = new ConcurrentHashMap<>();
        messageFuture = new ConcurrentHashMap<>();
    }

    /**
     * Retrieves the single instance of this class.
     */
    public static MessageBroker getInstance() {
        return MessageBrokerHolder.instance;
    }

    @Override
    public <T> void subscribeEvent(Class<? extends Event<T>> type, Subscriber m) {
        subscriberByMessage.putIfAbsent(type, new ConcurrentLinkedQueue<>());
        subscriberByMessage.get(type).add(m);
    }

    @Override
    public void subscribeBroadcast(Class<? extends Broadcast> type, Subscriber m) {
        subscriberByMessage.putIfAbsent(type, new ConcurrentLinkedQueue<>());
        subscriberByMessage.get(type).add(m);
    }

    @Override
    public <T> void complete(Event<T> e, T result) {
        messageFuture.get(e).resolve(result);
        messageFuture.remove(e);
    }

    @Override
    public void sendBroadcast(Broadcast b) {
        ConcurrentLinkedQueue<Subscriber> subscribersQueue = subscriberByMessage.get(b.getClass());
        if (subscribersQueue == null)
            return;
        synchronized (subscribersQueue) {
            for (Subscriber subscriber : subscribersQueue) {
                BlockingQueue<Message> q=messagePerSubscriber.get(subscriber);
                if (q != null)
                    q.add(b);
            }
        }
    }


    @Override
    public <T> Future<T> sendEvent(Event<T> e) {
        Subscriber subscriber;
        Future<T> future = new Future<>();
        ConcurrentLinkedQueue<Subscriber> subscribersQueue = subscriberByMessage.get(e.getClass());
        if (subscribersQueue == null) {
            future.resolve(null);
            return null;
        }
        synchronized (subscribersQueue) {
            if (subscribersQueue.isEmpty()) {
                future.resolve(null);
                return null;
            }
            subscriber = subscribersQueue.poll();
            subscribersQueue.add(subscriber);
        }

        messageFuture.put(e, future);
        synchronized (subscriber) {
            if (messagePerSubscriber.get(subscriber) == null) {
                future.resolve(null);
                return null;
            }
            messagePerSubscriber.get(subscriber).add(e);
        }
        return future;
    }

    @Override
    public void unregister(Subscriber m) {
        if (messagePerSubscriber.get(m) != null) {
            for (ConcurrentLinkedQueue<Subscriber> subscriberQueue : subscriberByMessage.values()) { //loop the queues
                synchronized (subscriberQueue) {
                    subscriberQueue.remove(m);
                }
            }
            BlockingQueue<Message> messages;
            synchronized (m) {
                messages = messagePerSubscriber.remove(m); //removing subscriber m
            }
            for (Message message : messages) { //clean the messages of the subscriber m
                if (messageFuture.containsKey(message)) {
                    if (!messageFuture.get(message).isDone()) {
                        messageFuture.get(message).resolve(null);
                    }
                }
            }


        }
    }


    @Override
    public void register(Subscriber m) {
        messagePerSubscriber.putIfAbsent(m, new LinkedBlockingQueue<>());
    }

    @Override
    public Message awaitMessage(Subscriber m) throws InterruptedException {
        BlockingQueue<Message> mQueue = messagePerSubscriber.get(m);
        return mQueue.take();
    }
}
