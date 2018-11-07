/*
The MIT License (MIT)

Copyright (c) 2017 Lancaster University.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#include "mbed.h"
#include "MbedSerial.h"
#include "ErrorNo.h"
#include "CodalComponent.h"
#include "CodalFiber.h"
#include "NotifyEvents.h"

namespace codal
{
    uint8_t Serial::status = 0;
    int Serial::baudrate = 0;

    namespace _mbed
    {
        /**
          * Constructor.
          * Create an instance of Serial
          *
          * @param tx the Pin to be used for transmission
          *
          * @param rx the Pin to be used for receiving data
          *
          * @param rxBufferSize the size of the buffer to be used for receiving bytes
          *
          * @param txBufferSize the size of the buffer to be used for transmitting bytes
          *
          * @code
          * Serial serial(USBTX, USBRX);
          * @endcode
          * @note the default baud rate is 115200. More API details can be found:
          *       -https://github.com/mbedmicro/mbed/blob/master/libraries/mbed/api/SerialBase.h
          *       -https://github.com/mbedmicro/mbed/blob/master/libraries/mbed/api/RawSerial.h
          *
          *       Buffers aren't allocated until the first send or receive respectively.
          */
        Serial::Serial(Pin &tx, Pin &rx, uint8_t rxBufferSize, uint8_t txBufferSize) : codal::Serial(tx, rx), mb::RawSerial((PinName)tx.name, (PinName)rx.name)
        {
            // + 1 so there is a usable buffer size, of the size the user requested.
            this->rxBuffSize = rxBufferSize + 1;
            this->txBuffSize = txBufferSize + 1;

            this->rxBuff = NULL;
            this->txBuff = NULL;

            this->rxBuffHead = 0;
            this->rxBuffTail = 0;

            this->txBuffHead = 0;
            this->txBuffTail = 0;

            this->rxBuffHeadMatch = -1;

            this->baud(CODAL_SERIAL_DEFAULT_BAUD_RATE);
        }

        /**
          * An internal interrupt callback for Serial configured for when a
          * character is received.
          *
          * Each time a character is received fill our circular buffer!
          */
        void Serial::dataReceived()
        {
            if(!(status & CODAL_SERIAL_RX_BUFF_INIT))
                return;

            //get the received character
            char c = getc();

            int delimeterOffset = 0;
            int delimLength = this->delimeters.length();

            //iterate through our delimeters (if any) to see if there is a match
            while(delimeterOffset < delimLength)
            {
                //fire an event if there is to block any waiting fibers
                if(this->delimeters.charAt(delimeterOffset) == c)
                    Event(DEVICE_ID_SERIAL, CODAL_SERIAL_EVT_DELIM_MATCH);

                delimeterOffset++;
            }

            uint16_t newHead = (rxBuffHead + 1) % rxBuffSize;

            //look ahead to our newHead value to see if we are about to collide with the tail
            if(newHead != rxBuffTail)
            {
                //if we are not, store the character, and update our actual head.
                this->rxBuff[rxBuffHead] = c;
                rxBuffHead = newHead;

                //if we have any fibers waiting for a specific number of characters, unblock them
                if(rxBuffHeadMatch >= 0)
                    if(rxBuffHead == rxBuffHeadMatch)
                    {
                        rxBuffHeadMatch = -1;
                        Event(DEVICE_ID_SERIAL, CODAL_SERIAL_EVT_HEAD_MATCH);
                    }
            }
            else
                //otherwise, our buffer is full, send an event to the user...
                Event(DEVICE_ID_SERIAL, CODAL_SERIAL_EVT_RX_FULL);
        }

        /**
          * An internal interrupt callback for Serial.
          *
          * Each time the Serial module's buffer is empty, write a character if we have
          * characters to write.
          */
        void Serial::dataWritten()
        {
            if(txBuffTail == txBuffHead || !(status & CODAL_SERIAL_TX_BUFF_INIT))
                return;

            //send our current char
            putc(txBuff[txBuffTail]);

            uint16_t nextTail = (txBuffTail + 1) % txBuffSize;

            //unblock any waiting fibers that are waiting for transmission to finish.
            if(nextTail == txBuffHead)
            {
                Event(DEVICE_ID_NOTIFY, CODAL_SERIAL_EVT_TX_EMPTY);
                detach(TxInterrupt);
            }

            //update our tail!
            txBuffTail = nextTail;
        }

        /**
          * An internal method to configure an interrupt on tx buffer and also
          * a best effort copy operation to move bytes from a user buffer to our txBuff
          *
          * @param string a pointer to the first character of the users' buffer.
          *
          * @param len the length of the string, and ultimately the maximum number of bytes
          *        that will be copied dependent on the state of txBuff
          *
          * @param mode determines whether to configure the current fiber context or not. If
          *             The mode is SYNC_SPINWAIT, the context will not be configured, otherwise
          *             no context will be configured.
          *
          * @return the number of bytes copied into the buffer.
          */
        int Serial::setTxInterrupt(uint8_t *string, int len, SerialMode mode)
        {
            int copiedBytes = 0;

            for(copiedBytes = 0; copiedBytes < len; copiedBytes++)
            {
                uint16_t nextHead = (txBuffHead + 1) % txBuffSize;
                if(nextHead != txBuffTail)
                {
                    this->txBuff[txBuffHead] = string[copiedBytes];
                    txBuffHead = nextHead;
                }
                else
                    break;
            }

            if(mode != SYNC_SPINWAIT)
                fiber_wake_on_event(DEVICE_ID_NOTIFY, CODAL_SERIAL_EVT_TX_EMPTY);

            //set the TX interrupt
            attach(TxInterrupt, &Serial::dataWritten);

            return copiedBytes;
        }

        /**
          * Locks the mutex so that others can't use this serial instance for reception
          */
        void Serial::lockRx()
        {
            status |= CODAL_SERIAL_RX_IN_USE;
        }

        /**
          * Locks the mutex so that others can't use this serial instance for transmission
          */
        void Serial::lockTx()
        {
            status |= CODAL_SERIAL_TX_IN_USE;
        }

        /**
          * Unlocks the mutex so that others can use this serial instance for reception
          */
        void Serial::unlockRx()
        {
            status &= ~CODAL_SERIAL_RX_IN_USE;
        }

        /**
          * Unlocks the mutex so that others can use this serial instance for transmission
          */
        void Serial::unlockTx()
        {
            status &= ~CODAL_SERIAL_TX_IN_USE;
        }

        /**
          * We do not want to always have our buffers initialised, especially if users to not
          * use them. We only bring them up on demand.
          */
        int Serial::initialiseRx()
        {
            if((status & CODAL_SERIAL_RX_BUFF_INIT))
            {
                //ensure that we receive no interrupts after freeing our buffer
                detach(RxInterrupt);
                free(this->rxBuff);
            }

            status &= ~CODAL_SERIAL_RX_BUFF_INIT;

            if((this->rxBuff = (uint8_t *)malloc(rxBuffSize)) == NULL)
                return DEVICE_NO_RESOURCES;

            this->rxBuffHead = 0;
            this->rxBuffTail = 0;

            //set the receive interrupt
            status |= CODAL_SERIAL_RX_BUFF_INIT;
            attach(RxInterrupt, &Serial::dataReceived);

            return DEVICE_OK;
        }

        /**
          * We do not want to always have our buffers initialised, especially if users to not
          * use them. We only bring them up on demand.
          */
        int Serial::initialiseTx()
        {
            if((status & CODAL_SERIAL_TX_BUFF_INIT))
            {
                //ensure that we receive no interrupts after freeing our buffer
                detach(TxInterrupt);
                free(this->txBuff);
            }

            status &= ~CODAL_SERIAL_TX_BUFF_INIT;

            if((this->txBuff = (uint8_t *)malloc(txBuffSize)) == NULL)
                return DEVICE_NO_RESOURCES;

            this->txBuffHead = 0;
            this->txBuffTail = 0;

            status |= CODAL_SERIAL_TX_BUFF_INIT;

            return DEVICE_OK;
        }

        /**
          * An internal method that either spin waits if mode is set to SYNC_SPINWAIT
          * or puts the fiber to sleep if the mode is set to SYNC_SLEEP
          *
          * @param mode the selected mode, one of: ASYNC, SYNC_SPINWAIT, SYNC_SLEEP
          */
        void Serial::send(SerialMode mode)
        {
            if(mode == SYNC_SPINWAIT)
                while(txBufferedSize() > 0);

            if(mode == SYNC_SLEEP)
                fiber_sleep(0);
        }

        /**
          * Reads a single character from the rxBuff
          *
          * @param mode the selected mode, one of: ASYNC, SYNC_SPINWAIT, SYNC_SLEEP. Each mode
          *        gives a different behaviour:
          *
          *            ASYNC - A character is read from the rxBuff if available, if there
          *                    are no characters to be read, a value of zero is returned immediately.
          *
          *            SYNC_SPINWAIT - A character is read from the rxBuff if available, if there
          *                            are no characters to be read, this method will spin
          *                            (lock up the processor) until a character is available.
          *
          *            SYNC_SLEEP - A character is read from the rxBuff if available, if there
          *                         are no characters to be read, the calling fiber sleeps
          *                         until there is a character available.
          *
          *         Defaults to SYNC_SLEEP.
          *
          * @return a character from the circular buffer, or DEVICE_NO_DATA is there
          *         are no characters in the buffer.
          */
        int Serial::getChar(SerialMode mode)
        {
            if(mode == ASYNC)
            {
                if(!isReadable())
                    return DEVICE_NO_DATA;
            }

            if(mode == SYNC_SPINWAIT)
                while(!isReadable());

            if(mode == SYNC_SLEEP)
            {
                if(!isReadable())
                    eventAfter(1, mode);
            }

            char c = rxBuff[rxBuffTail];

            rxBuffTail = (rxBuffTail + 1) % rxBuffSize;

            return c;
        }

        /**
          * An internal method that copies values from a circular buffer to a linear buffer.
          *
          * @param circularBuff a pointer to the source circular buffer
          *
          * @param circularBuffSize the size of the circular buffer
          *
          * @param linearBuff a pointer to the destination linear buffer
          *
          * @param tailPosition the tail position in the circular buffer you want to copy from
          *
          * @param headPosition the head position in the circular buffer you want to copy to
          *
          * @note this method assumes that the linear buffer has the appropriate amount of
          *       memory to contain the copy operation
          */
        void Serial::circularCopy(uint8_t *circularBuff, uint8_t circularBuffSize, uint8_t *linearBuff, uint16_t tailPosition, uint16_t headPosition)
        {
            int toBuffIndex = 0;

            while(tailPosition != headPosition)
            {
                linearBuff[toBuffIndex++] = circularBuff[tailPosition];

                tailPosition = (tailPosition + 1) % circularBuffSize;
            }
        }

        /**
          * Sends a single character over the serial line.
          *
          * @param c the character to send
          *
          * @param mode the selected mode, one of: ASYNC, SYNC_SPINWAIT, SYNC_SLEEP. Each mode
          *        gives a different behaviour:
          *
          *            ASYNC - the character is copied into the txBuff and returns immediately.
          *
          *            SYNC_SPINWAIT - the character is copied into the txBuff and this method
          *                            will spin (lock up the processor) until the character has
          *                            been sent.
          *
          *            SYNC_SLEEP - the character is copied into the txBuff and the fiber sleeps
          *                         until the character has been sent. This allows other fibers
          *                         to continue execution.
          *
          *         Defaults to SYNC_SLEEP.
          *
          * @return the number of bytes written, or DEVICE_SERIAL_IN_USE if another fiber
          *         is using the serial instance for transmission.
          */
        int Serial::sendChar(char c, SerialMode mode)
        {
            if(txInUse())
                return DEVICE_SERIAL_IN_USE;

            lockTx();

            //lazy initialisation of our tx buffer
            if(!(status & CODAL_SERIAL_TX_BUFF_INIT))
            {
                int result = initialiseTx();

                if(result != DEVICE_OK)
                    return result;
            }

            uint8_t toTransmit[2] =  { c, '\0'};

            int bytesWritten = setTxInterrupt(toTransmit, 1, mode);

            send(mode);

            unlockTx();

            return bytesWritten;
        }

        /**
          * Sends a ManagedString over the serial line.
          *
          * @param s the string to send
          *
          * @param mode the selected mode, one of: ASYNC, SYNC_SPINWAIT, SYNC_SLEEP. Each mode
          *        gives a different behaviour:
          *
          *            ASYNC - bytes are copied into the txBuff and returns immediately.
          *
          *            SYNC_SPINWAIT - bytes are copied into the txBuff and this method
          *                            will spin (lock up the processor) until all bytes
          *                            have been sent.
          *
          *            SYNC_SLEEP - bytes are copied into the txBuff and the fiber sleeps
          *                         until all bytes have been sent. This allows other fibers
          *                         to continue execution.
          *
          *         Defaults to SYNC_SLEEP.
          *
          * @return the number of bytes written, DEVICE_SERIAL_IN_USE if another fiber
          *         is using the serial instance for transmission, DEVICE_INVALID_PARAMETER
          *         if buffer is invalid, or the given bufferLen is <= 0.
          */
        int Serial::send(ManagedString s, SerialMode mode)
        {
            return send((uint8_t *)s.toCharArray(), s.length(), mode);
        }

        /**
          * Sends a buffer of known length over the serial line.
          *
          * @param buffer a pointer to the first character of the buffer
          *
          * @param len the number of bytes that are safely available to read.
          *
          * @param mode the selected mode, one of: ASYNC, SYNC_SPINWAIT, SYNC_SLEEP. Each mode
          *        gives a different behaviour:
          *
          *            ASYNC - bytes are copied into the txBuff and returns immediately.
          *
          *            SYNC_SPINWAIT - bytes are copied into the txBuff and this method
          *                            will spin (lock up the processor) until all bytes
          *                            have been sent.
          *
          *            SYNC_SLEEP - bytes are copied into the txBuff and the fiber sleeps
          *                         until all bytes have been sent. This allows other fibers
          *                         to continue execution.
          *
          *         Defaults to SYNC_SLEEP.
          *
          * @return the number of bytes written, DEVICE_SERIAL_IN_USE if another fiber
          *         is using the serial instance for transmission, DEVICE_INVALID_PARAMETER
          *         if buffer is invalid, or the given bufferLen is <= 0.
          */
        int Serial::send(uint8_t *buffer, int bufferLen, SerialMode mode)
        {
            if(txInUse())
                return DEVICE_SERIAL_IN_USE;

            if(bufferLen <= 0 || buffer == NULL)
                return DEVICE_INVALID_PARAMETER;

            lockTx();

            //lazy initialisation of our tx buffer
            if(!(status & CODAL_SERIAL_TX_BUFF_INIT))
            {
                int result = initialiseTx();

                if(result != DEVICE_OK)
                    return result;
            }

            bool complete = false;
            int bytesWritten = 0;

            while(!complete)
            {
                bytesWritten += setTxInterrupt(buffer + bytesWritten, bufferLen - bytesWritten, mode);
                send(mode);

                if(mode == ASYNC || bytesWritten >= bufferLen)
                    complete = true;
            }

            unlockTx();

            return bytesWritten;
        }

        /**
          * Reads a single character from the rxBuff
          *
          * @param mode the selected mode, one of: ASYNC, SYNC_SPINWAIT, SYNC_SLEEP. Each mode
          *        gives a different behaviour:
          *
          *            ASYNC - A character is read from the rxBuff if available, if there
          *                    are no characters to be read, a value of DEVICE_NO_DATA is returned immediately.
          *
          *            SYNC_SPINWAIT - A character is read from the rxBuff if available, if there
          *                            are no characters to be read, this method will spin
          *                            (lock up the processor) until a character is available.
          *
          *            SYNC_SLEEP - A character is read from the rxBuff if available, if there
          *                         are no characters to be read, the calling fiber sleeps
          *                         until there is a character available.
          *
          *         Defaults to SYNC_SLEEP.
          *
          * @return a character, DEVICE_SERIAL_IN_USE if another fiber is using the serial instance for reception,
          *         DEVICE_NO_RESOURCES if buffer allocation did not complete successfully, or DEVICE_NO_DATA if
          *         the rx buffer is empty and the mode given is ASYNC.
          */
        int Serial::read(SerialMode mode)
        {
            if(rxInUse())
                return DEVICE_SERIAL_IN_USE;

            lockRx();

            //lazy initialisation of our buffers
            if(!(status & CODAL_SERIAL_RX_BUFF_INIT))
            {
                int result = initialiseRx();

                if(result != DEVICE_OK)
                    return result;
            }

            int c = getChar(mode);

            unlockRx();

            return c;
        }

        /**
          * Reads multiple characters from the rxBuff and returns them as a ManagedString
          *
          * @param size the number of characters to read.
          *
          * @param mode the selected mode, one of: ASYNC, SYNC_SPINWAIT, SYNC_SLEEP. Each mode
          *        gives a different behaviour:
          *
          *            ASYNC - If the desired number of characters are available, this will return
          *                    a ManagedString with the expected size. Otherwise, it will read however
          *                    many characters there are available.
          *
          *            SYNC_SPINWAIT - If the desired number of characters are available, this will return
          *                            a ManagedString with the expected size. Otherwise, this method will spin
          *                            (lock up the processor) until the desired number of characters have been read.
          *
          *            SYNC_SLEEP - If the desired number of characters are available, this will return
          *                         a ManagedString with the expected size. Otherwise, the calling fiber sleeps
          *                         until the desired number of characters have been read.
          *
          *         Defaults to SYNC_SLEEP.
          *
          * @return A ManagedString, or an empty ManagedString if an error was encountered during the read.
          */
        ManagedString Serial::read(int size, SerialMode mode)
        {
            uint8_t buff[size + 1];

            memclr(&buff, size + 1);

            int returnedSize = read((uint8_t *)buff, size, mode);

            if(returnedSize <= 0)
                return ManagedString();

            return ManagedString((char *)buff, returnedSize);
        }

        /**
          * Reads multiple characters from the rxBuff and fills a user buffer.
          *
          * @param buffer a pointer to a user allocated buffer.
          *
          * @param bufferLen the amount of data that can be safely stored
          *
          * @param mode the selected mode, one of: ASYNC, SYNC_SPINWAIT, SYNC_SLEEP. Each mode
          *        gives a different behaviour:
          *
          *            ASYNC - If the desired number of characters are available, this will fill
          *                    the given buffer. Otherwise, it will fill the buffer with however
          *                    many characters there are available.
          *
          *            SYNC_SPINWAIT - If the desired number of characters are available, this will fill
          *                            the given buffer. Otherwise, this method will spin (lock up the processor)
          *                            and fill the buffer until the desired number of characters have been read.
          *
          *            SYNC_SLEEP - If the desired number of characters are available, this will fill
          *                         the given buffer. Otherwise, the calling fiber sleeps
          *                         until the desired number of characters have been read.
          *
          *         Defaults to SYNC_SLEEP.
          *
          * @return the number of characters read, or DEVICE_SERIAL_IN_USE if another fiber
          *         is using the instance for receiving.
          */
        int Serial::read(uint8_t *buffer, int bufferLen, SerialMode mode)
        {
            if(rxInUse())
                return DEVICE_SERIAL_IN_USE;

            lockRx();

            //lazy initialisation of our rx buffer
            if(!(status & CODAL_SERIAL_RX_BUFF_INIT))
            {
                int result = initialiseRx();

                if(result != DEVICE_OK)
                    return result;
            }

            int bufferIndex = 0;

            int temp = 0;

            if(mode == ASYNC)
            {
                while((temp = getChar(mode)) != DEVICE_NO_DATA && bufferIndex < bufferLen)
                {
                    buffer[bufferIndex] = (char)temp;
                    bufferIndex++;
                }
            }

            if(mode == SYNC_SPINWAIT)
            {
                while(bufferIndex < bufferLen)
                {
                    buffer[bufferIndex] = (char)getChar(mode);
                    bufferIndex++;
                }
            }

            if(mode == SYNC_SLEEP)
            {
                if(bufferLen > rxBufferedSize())
                    eventAfter(bufferLen - rxBufferedSize(), mode);

                while(bufferIndex < bufferLen)
                {
                    buffer[bufferIndex] = (char)getChar(mode);
                    bufferIndex++;
                }
            }

            unlockRx();

            return bufferIndex;
        }


        /**
          * Reads until one of the delimeters matches a character in the rxBuff
          *
          * @param delimeters a ManagedString containing a sequence of delimeter characters e.g. ManagedString("\r\n")
          *
          * @param mode the selected mode, one of: ASYNC, SYNC_SPINWAIT, SYNC_SLEEP. Each mode
          *        gives a different behaviour:
          *
          *            ASYNC - If one of the delimeters matches a character already in the rxBuff
          *                    this method will return a ManagedString up to the delimeter.
          *                    Otherwise, it will return an Empty ManagedString.
          *
          *            SYNC_SPINWAIT - If one of the delimeters matches a character already in the rxBuff
          *                            this method will return a ManagedString up to the delimeter.
          *                            Otherwise, this method will spin (lock up the processor) until a
          *                            received character matches one of the delimeters.
          *
          *            SYNC_SLEEP - If one of the delimeters matches a character already in the rxBuff
          *                         this method will return a ManagedString up to the delimeter.
          *                         Otherwise, the calling fiber sleeps until a character matching one
          *                         of the delimeters is seen.
          *
          *         Defaults to SYNC_SLEEP.
          *
          * @return A ManagedString containing the characters up to a delimeter, or an Empty ManagedString,
          *         if another fiber is currently using this instance for reception.
          *
          * @note delimeters are matched on a per byte basis.
          */
        ManagedString Serial::readUntil(ManagedString delimeters, SerialMode mode)
        {

            if(rxInUse())
                return ManagedString();

            //lazy initialisation of our rx buffer
            if(!(status & CODAL_SERIAL_RX_BUFF_INIT))
            {
                int result = initialiseRx();

                if(result != DEVICE_OK)
                    return result;
            }

            lockRx();

            int localTail = rxBuffTail;
            int preservedTail = rxBuffTail;

            int foundIndex = -1;

            //ASYNC mode just iterates through our stored characters checking for any matches.
            while(localTail != rxBuffHead && foundIndex  == -1)
            {
                //we use localTail to prevent modification of the actual tail.
                char c = rxBuff[localTail];

                for(int delimeterIterator = 0; delimeterIterator < delimeters.length(); delimeterIterator++)
                    if(delimeters.charAt(delimeterIterator) == c)
                        foundIndex = localTail;

                localTail = (localTail + 1) % rxBuffSize;
            }

            //if our mode is SYNC_SPINWAIT and we didn't see any matching characters in our buffer
            //spin until we find a match!
            if(mode == SYNC_SPINWAIT)
            {
                while(foundIndex == -1)
                {
                    while(localTail == rxBuffHead);

                    char c = rxBuff[localTail];

                    for(int delimeterIterator = 0; delimeterIterator < delimeters.length(); delimeterIterator++)
                        if(delimeters.charAt(delimeterIterator) == c)
                            foundIndex = localTail;

                    localTail = (localTail + 1) % rxBuffSize;
                }
            }

            //if our mode is SYNC_SLEEP, we set up an event to be fired when we see a
            //matching character.
            if(mode == SYNC_SLEEP && foundIndex == -1)
            {
                eventOn(delimeters, mode);

                foundIndex = rxBuffHead - 1;

                this->delimeters = ManagedString();
            }

            if(foundIndex >= 0)
            {
                //calculate our local buffer size
                int localBuffSize = (preservedTail > foundIndex) ? (rxBuffSize - preservedTail) + foundIndex : foundIndex - preservedTail;

                uint8_t localBuff[localBuffSize + 1];

                memclr(&localBuff, localBuffSize + 1);

                circularCopy(rxBuff, rxBuffSize, localBuff, preservedTail, foundIndex);

                //plus one for the character we listened for...
                rxBuffTail = (rxBuffTail + localBuffSize + 1) % rxBuffSize;

                unlockRx();

                return ManagedString((char *)localBuff, localBuffSize);
            }

            unlockRx();

            return ManagedString();
        }

        /**
          * A wrapper around the inherited method "baud" so we can trap the baud rate
          * as it changes and restore it if redirect() is called.
          *
          * @param baudrate the new baudrate. See:
          *         - https://github.com/mbedmicro/mbed/blob/master/libraries/mbed/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822/serial_api.c
          *        for permitted baud rates.
          *
          * @return DEVICE_INVALID_PARAMETER if baud rate is less than 0, otherwise DEVICE_OK.
          *
          * @note the underlying implementation chooses the first allowable rate at or above that requested.
          */
        int Serial::baud(int baudrate)
        {
            if(baudrate < 0)
                return DEVICE_INVALID_PARAMETER;

            this->baudrate = baudrate;

            RawSerial::baud(baudrate);

            return DEVICE_OK;
        }

        /**
          * A way of dynamically configuring the serial instance to use pins other than USBTX and USBRX.
          *
          * @param tx the new transmission pin.
          *
          * @param rx the new reception pin.
          *
          * @return DEVICE_SERIAL_IN_USE if another fiber is currently transmitting or receiving, otherwise DEVICE_OK.
          */
        int Serial::redirect(PinName tx, PinName rx)
        {
            if(txInUse() || rxInUse())
                return DEVICE_SERIAL_IN_USE;

            lockTx();
            lockRx();

            if(txBufferedSize() > 0)
                detach(TxInterrupt);

            detach(RxInterrupt);

            serial_init(&_serial, tx, rx);

            attach(RxInterrupt, &Serial::dataReceived);

            if(txBufferedSize() > 0)
                attach(TxInterrupt, &Serial::dataWritten);

            this->baud(this->baudrate);

            unlockRx();
            unlockTx();

            return DEVICE_OK;
        }

        /**
          * Configures an event to be fired after "len" characters.
          *
          * Will generate an event with the ID: DEVICE_ID_SERIAL and the value CODAL_SERIAL_EVT_HEAD_MATCH.
          *
          * @param len the number of characters to wait before triggering the event.
          *
          * @param mode the selected mode, one of: ASYNC, SYNC_SPINWAIT, SYNC_SLEEP. Each mode
          *        gives a different behaviour:
          *
          *            ASYNC - Will configure the event and return immediately.
          *
          *            SYNC_SPINWAIT - will return DEVICE_INVALID_PARAMETER
          *
          *            SYNC_SLEEP - Will configure the event and block the current fiber until the
          *                         event is received.
          *
          * @return DEVICE_INVALID_PARAMETER if the mode given is SYNC_SPINWAIT, otherwise DEVICE_OK.
          */
        int Serial::eventAfter(int len, SerialMode mode)
        {
            if(mode == SYNC_SPINWAIT)
                return DEVICE_INVALID_PARAMETER;

            //configure our head match...
            this->rxBuffHeadMatch = (rxBuffHead + len) % rxBuffSize;

            //block!
            if(mode == SYNC_SLEEP)
                fiber_wait_for_event(DEVICE_ID_SERIAL, CODAL_SERIAL_EVT_HEAD_MATCH);

            return DEVICE_OK;
        }

        /**
          * Configures an event to be fired on a match with one of the delimeters.
          *
          * Will generate an event with the ID: DEVICE_ID_SERIAL and the value CODAL_SERIAL_EVT_DELIM_MATCH.
          *
          * @param delimeters the characters to match received characters against e.g. ManagedString("\n")
          *
          * @param mode the selected mode, one of: ASYNC, SYNC_SPINWAIT, SYNC_SLEEP. Each mode
          *        gives a different behaviour:
          *
          *            ASYNC - Will configure the event and return immediately.
          *
          *            SYNC_SPINWAIT - will return DEVICE_INVALID_PARAMETER
          *
          *            SYNC_SLEEP - Will configure the event and block the current fiber until the
          *                         event is received.
          *
          * @return DEVICE_INVALID_PARAMETER if the mode given is SYNC_SPINWAIT, otherwise DEVICE_OK.
          *
          * @note delimeters are matched on a per byte basis.
          */
        int Serial::eventOn(ManagedString delimeters, SerialMode mode)
        {
            if(mode == SYNC_SPINWAIT)
                return DEVICE_INVALID_PARAMETER;

            //configure our head match...
            this->delimeters = delimeters;

            //block!
            if(mode == SYNC_SLEEP)
                fiber_wait_for_event(DEVICE_ID_SERIAL, CODAL_SERIAL_EVT_DELIM_MATCH);

            return DEVICE_OK;
        }

        /**
          * Determines whether there is any data waiting in our Rx buffer.
          *
          * @return 1 if we have space, 0 if we do not.
          *
          * @note We do not wrap the super's readable() method as we don't want to
          *       interfere with communities that use manual calls to serial.readable().
          */
        int Serial::isReadable()
        {
            return (rxBuffTail != rxBuffHead) ? 1 : 0;
        }

        /**
          * Determines if we have space in our txBuff.
          *
          * @return 1 if we have space, 0 if we do not.
          *
          * @note We do not wrap the super's writeable() method as we don't want to
          *       interfere with communities that use manual calls to serial.writeable().
          */
        int Serial::isWriteable()
        {
            return (txBuffHead != (txBuffTail - 1)) ? 1 : 0;
        }

        /**
          * Reconfigures the size of our rxBuff
          *
          * @param size the new size for our rxBuff
          *
          * @return DEVICE_SERIAL_IN_USE if another fiber is currently using this instance
          *         for reception, otherwise DEVICE_OK.
          */
        int Serial::setRxBufferSize(uint8_t size)
        {
            if(rxInUse())
                return DEVICE_SERIAL_IN_USE;

            lockRx();

            // + 1 so there is a usable buffer size, of the size the user requested.
            this->rxBuffSize = size + 1;

            int result = initialiseRx();

            unlockRx();

            return result;
        }

        /**
          * Reconfigures the size of our txBuff
          *
          * @param size the new size for our txBuff
          *
          * @return DEVICE_SERIAL_IN_USE if another fiber is currently using this instance
          *         for transmission, otherwise DEVICE_OK.
          */
        int Serial::setTxBufferSize(uint8_t size)
        {
            if(txInUse())
                return DEVICE_SERIAL_IN_USE;

            lockTx();

            // + 1 so there is a usable buffer size, of the size the user requested.
            this->txBuffSize = size + 1;

            int result = initialiseTx();

            unlockTx();

            return result;
        }

        /**
          * The size of our rx buffer in bytes.
          *
          * @return the current size of rxBuff in bytes
          */
        int Serial::getRxBufferSize()
        {
            return this->rxBuffSize;
        }

        /**
          * The size of our tx buffer in bytes.
          *
          * @return the current size of txBuff in bytes
          */
        int Serial::getTxBufferSize()
        {
            return this->txBuffSize;
        }

        /**
          * Sets the tail to match the head of our circular buffer for reception,
          * effectively clearing the reception buffer.
          *
          * @return DEVICE_SERIAL_IN_USE if another fiber is currently using this instance
          *         for reception, otherwise DEVICE_OK.
          */
        int Serial::clearRxBuffer()
        {
            if(rxInUse())
                return DEVICE_SERIAL_IN_USE;

            lockRx();

            rxBuffTail = rxBuffHead;

            unlockRx();

            return DEVICE_OK;
        }

        /**
          * Sets the tail to match the head of our circular buffer for transmission,
          * effectively clearing the transmission buffer.
          *
          * @return DEVICE_SERIAL_IN_USE if another fiber is currently using this instance
          *         for transmission, otherwise DEVICE_OK.
          */
        int Serial::clearTxBuffer()
        {
            if(txInUse())
                return DEVICE_SERIAL_IN_USE;

            lockTx();

            txBuffTail = txBuffHead;

            unlockTx();

            return DEVICE_OK;
        }

        /**
          * The number of bytes currently stored in our rx buffer waiting to be digested,
          * by the user.
          *
          * @return The currently buffered number of bytes in our rxBuff.
          */
        int Serial::rxBufferedSize()
        {
            if(rxBuffTail > rxBuffHead)
                return (rxBuffSize - rxBuffTail) + rxBuffHead;

            return rxBuffHead - rxBuffTail;
        }

        /**
          * The number of bytes currently stored in our tx buffer waiting to be transmitted
          * by the hardware.
          *
          * @return The currently buffered number of bytes in our txBuff.
          */
        int Serial::txBufferedSize()
        {
            if(txBuffTail > txBuffHead)
                return (txBuffSize - txBuffTail) + txBuffHead;

            return txBuffHead - txBuffTail;
        }

        /**
          * Determines if the serial bus is currently in use by another fiber for reception.
          *
          * @return The state of our mutex lock for reception.
          *
          * @note Only one fiber can call read at a time
          */
        int Serial::rxInUse()
        {
            return (status & CODAL_SERIAL_RX_IN_USE);
        }

        /**
          * Determines if the serial bus is currently in use by another fiber for transmission.
          *
          * @return The state of our mutex lock for transmition.
          *
          * @note Only one fiber can call send at a time
          */
        int Serial::txInUse()
        {
            return (status & CODAL_SERIAL_TX_IN_USE);
        }

        /**
          * attaches an interrupt.
          *
          * @param interruptType one of RxInterrupt or TxInterrupt
          *
          * @return DEVICE_OK on success.
          */
        int Serial::attach(SerialInterruptType interruptType, void (Serial::*fp)())
        {
            mb::RawSerial::attach(this, fp, (interruptType == RxInterrupt) ? RxIrq : TxIrq);
            return DEVICE_OK;
        }

        /**
          * Detaches a previously configured interrupt
          *
          * @param interruptType one of RxInterrupt or TxInterrupt
          *
          * @return DEVICE_OK on success.
          */
        int Serial::detach(SerialInterruptType interruptType)
        {
            //we detach by sending a bad value to attach, for some weird reason...
            mb::RawSerial::attach((Serial *)NULL, &Serial::dataReceived,(interruptType == RxInterrupt) ? RxIrq : TxIrq);
            return DEVICE_OK;
        }
    }
}