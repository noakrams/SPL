package bgu.spl.net.impl.stomp;
import java.util.concurrent.locks.ReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;
public class Lock {
    private final ReadWriteLock lock = new ReentrantReadWriteLock();

    void lockReader() {
        lock.readLock().lock();
    }

    void lockWriter() {
        lock.writeLock().lock();
    }

    void lockReaderRelease() {
        lock.readLock().unlock();
    }

    void lockWriterRelease() {
        lock.writeLock().unlock();
    }
}
