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

#ifndef CM_SPI_H
#define CM_SPI_H

#include "CodalConfig.h"
#include "SPI.h"
#include "Pin.h"

namespace codal
{
    namespace _cm
    {
#ifdef TODO
        class SPI;
        struct SPI_HandleWithParent : public SPI_HandleTypeDef
        {
            SPI *spi_parent;
        };
#endif  //  TODO

        /**
         * Class definition for SPI service, derived from ARM mbed.
         */
        class SPI : public codal::SPI
        {
        protected:
            Pin *mosi, *miso, *sclk;
            uint32_t freq;

#ifdef TODO
            SPI_HandleWithParent spi;
            DMA_HandleTypeDef hdma_tx;
            DMA_HandleTypeDef hdma_rx;
#endif  //  TODO

            PVoidCallback doneHandler;
            void *doneHandlerArg;

            bool needsInit;
            uint8_t rxCh, txCh;
            uint16_t transferCompleteEventCode;

            void complete();
            void init();

        public:
            static void _complete(uint32_t instance);
            static void _irq(uint32_t instance);

            /**
             * Initialize SPI instance with given pins.
             *
             * Default setup is 1 MHz, 8 bit, mode 0.
             */
            SPI(codal::Pin &mosi, codal::Pin &miso, codal::Pin &sclk);

            /** Set the frequency of the SPI interface
             *
             * @param frequency The bus frequency in hertz
             */
            virtual int setFrequency(uint32_t frequency);

            /** Set the mode of the SPI interface
             *
             * @param mode Clock polarity and phase mode (0 - 3)
             * @param bits Number of bits per SPI frame (4 - 16)
             *
             * @code
             * mode | POL PHA
             * -----+--------
             *   0  |  0   0
             *   1  |  0   1
             *   2  |  1   0
             *   3  |  1   1
             * @endcode
             */
            virtual int setMode(int mode, int bits = 8);

            /**
             * Writes the given byte to the SPI bus.
             *
             * The CPU will wait until the transmission is complete.
             *
             * @param data The data to write.
             * @return Response from the SPI slave or DEVICE_SPI_ERROR if the the write request failed.
             */
            virtual int write(int data);

            /**
             * Writes and reads from the SPI bus concurrently. Waits un-scheduled for transfer to finish.
             *
             * Either buffer can be NULL.
             */
            virtual int transfer(const uint8_t *txBuffer, uint32_t txSize, uint8_t *rxBuffer,
                                uint32_t rxSize);

            virtual int startTransfer(const uint8_t *txBuffer, uint32_t txSize, uint8_t *rxBuffer,
                                    uint32_t rxSize, PVoidCallback doneHandler, void *arg);
        };

    }  //  namespace _cm
} // namespace codal

#endif  //  CM_SPI_H
