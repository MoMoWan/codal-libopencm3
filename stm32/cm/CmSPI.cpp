//  Based on https://github.com/lancaster-university/codal-stm32/blob/master/src/ZSPI.cpp
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
#include "CodalConfig.h"
#include "CmSPI.h"
#include "ErrorNo.h"
#include "CodalDmesg.h"
#include "Timer.h"
#include "MessageBus.h"
#include "Event.h"
#include "CodalFiber.h"

#include "dma.h"
#include "pinmap.h"
#include "PeripheralPins.h"

#define HAL_OK 0
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

//#define LOG DMESG
#define LOG(...) ((void)0)

namespace codal {
    namespace _cm {
        static SPI *_instances[4];
        #define ZERO(f) memset(&f, 0, sizeof(f))

        uint32_t _codal_setup_pin(Pin *p, uint32_t prev, const PinMap *map) {
            if (!p) { return 0; }
            auto pin = p->name;
            uint32_t peri = pinmap_peripheral(pin, map);
            if (peri == (uint32_t)NC) { return peri; }
            
            auto mode = pinmap_mode(pin, map);
            auto cnf = pinmap_cnf(pin, map);
            p->setup(mode, cnf);
            CODAL_ASSERT(!prev || prev == peri);
            return peri;
        }

        void SPI::init() {
            auto res = HAL_OK;
            if (!needsInit) { return; }
            needsInit = false;
            // if (!spi.Instance) {
            uint32_t instance = _codal_setup_pin(sclk, 0, PinMap_SPI_SCLK);
            instance = _codal_setup_pin(miso, 0, PinMap_SPI_MISO);
            instance = _codal_setup_pin(mosi, 0, PinMap_SPI_MOSI);
            instance = _codal_setup_pin(nss, 0, PinMap_SPI_SSEL);
            // spi.Instance = (SPI_TypeDef *)instance; }
            LOG("SPI instance %p", instance);
#ifdef TODO
            spi.Init.Direction = SPI_DIRECTION_2LINES;
            spi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
            spi.Init.CRCPolynomial = 7;
            spi.Init.DataSize = SPI_DATASIZE_8BIT;
            spi.Init.FirstBit = SPI_FIRSTBIT_MSB;
            spi.Init.NSS = SPI_NSS_SOFT;
            spi.Init.TIMode = SPI_TIMODE_DISABLE;
            spi.Init.Mode = SPI_MODE_MASTER;
            if (mosi && !hdma_tx.Instance) {
                dma_init((uint32_t)spi.Instance, DMA_TX, &hdma_tx, 0);
                __HAL_LINKDMA(&spi, hdmatx, hdma_tx);
            }
            if (miso && !hdma_rx.Instance) {
                dma_init((uint32_t)spi.Instance, DMA_RX, &hdma_rx, 0);
                __HAL_LINKDMA(&spi, hdmarx, hdma_rx);
            }
            auto pclkHz = enable_clock((uint32_t) spi.Instance);
            for (int i = 0; baudprescaler[i]; i += 2) {
                spi.Init.BaudRatePrescaler = baudprescaler[i];
                if (pclkHz / baudprescaler[i + 1] <= freq)
                    break;
            }
            spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
            auto res = HAL_SPI_Init(&spi);
#endif  //  TODO
            CODAL_ASSERT(res == HAL_OK);
        }

        SPI::SPI(Pin &mosi, Pin &miso, Pin &sclk, Pin &nss) : codal::SPI() {
            this->mosi = &mosi;
            this->miso = &miso;
            this->sclk = &sclk;
            this->nss = &nss;
            // ZERO(spi); ZERO(hdma_tx); ZERO(hdma_rx);
            this->needsInit = true;
            this->transferCompleteEventCode = codal::allocateNotifyEvent();
            for (unsigned i = 0; i < ARRAY_SIZE(_instances); ++i) {
                if (_instances[i] == NULL) {
                    _instances[i] = this;
                    break;
                }
            }
        }

        int SPI::setFrequency(uint32_t frequency) {
            freq = frequency;
            needsInit = true;
            return DEVICE_OK;
        }

        int SPI::setMode(int mode, int bits) {
#ifdef TODO
            spi.Init.CLKPhase = mode & 1 ? SPI_PHASE_2EDGE : SPI_PHASE_1EDGE;
            spi.Init.CLKPolarity = mode & 2 ? SPI_POLARITY_HIGH : SPI_POLARITY_LOW;
#endif  //  TODO
            needsInit = true;
            CODAL_ASSERT(bits == 8);
            return DEVICE_OK;
        }

        int SPI::write(int data) {
            rxCh = 0;
            txCh = data;
#ifdef TODO
            if (transfer(&txCh, 1, &rxCh, 1) < 0) { return DEVICE_SPI_ERROR; }
#endif  //  TODO
            return rxCh;
        }

        int SPI::transfer(const uint8_t *txBuffer, uint32_t txSize, uint8_t *rxBuffer, uint32_t rxSize) {    
            return 0; ////TODO
#ifdef TODO
            fiber_wake_on_event(DEVICE_ID_NOTIFY, transferCompleteEventCode);
            auto res = startTransfer(txBuffer, txSize, rxBuffer, rxSize, NULL, NULL);
            LOG("SPI ->");
            schedule();
            LOG("SPI <-");
            return res;
#endif  //  TODO
        }

        int SPI::startTransfer(const uint8_t *txBuffer, uint32_t txSize, uint8_t *rxBuffer,
                                uint32_t rxSize, PVoidCallback doneHandler, void *arg) {
            int res = HAL_OK;
            init();
            LOG("SPI start %p/%d %p/%d D=%p", txBuffer, txSize, rxBuffer, rxSize, doneHandler);
            this->doneHandler = doneHandler;
            this->doneHandlerArg = arg;
            if (txSize && rxSize) {
                CODAL_ASSERT(txSize == rxSize); // we could support this if needed
#ifdef TODO
                res = HAL_SPI_TransmitReceive_DMA(&spi, (uint8_t *)txBuffer, rxBuffer, txSize);
#endif  //  TODO
            } else if (txSize) {
#ifdef TODO
                res = HAL_SPI_Transmit_DMA(&spi, (uint8_t *)txBuffer, txSize);
#endif  //  TODO
            } else if (rxSize) {
#ifdef TODO
                res = HAL_SPI_Receive_DMA(&spi, rxBuffer, rxSize);
#endif  //  TODO
            } else {
                return 0; // nothing to do
            }
            CODAL_ASSERT(res == HAL_OK);
            return 0;
        }

        void SPI::complete() {
            LOG("SPI complete D=%p", doneHandler);
            if (doneHandler) {
                PVoidCallback done = doneHandler;
                doneHandler = NULL;
                //create_fiber(done, doneHandlerArg);
                done(doneHandlerArg);
            } else {
                Event(DEVICE_ID_NOTIFY_ONE, transferCompleteEventCode);
            }
        }

        void SPI::_complete(SPI *instance) {
            //  LOG("SPI complete %p", instance);
            SPI *inst = _find(instance);
            if (inst) { inst->complete(); }
            //  TODO: Handle not found.
        }

        void SPI::_irq(SPI *instance) {
            //  LOG("SPI IRQ %p", instance);
            SPI *inst = _find(instance);
            if (inst) { 
#ifdef TODO
                HAL_SPI_IRQHandler(inst);
#endif  //  TODO
            }
            //  TODO: Handle not found.
        }

        SPI *SPI::_find(SPI *instance) {
            //  Warning: Called by interrupt service routine.
            //  LOG("SPI find %p", instance);
            if (!instance) { return NULL; }  //  Not found.
            if (!instance->mosi || !instance->miso || 
                !instance->sclk || !instance->nss)
                { return NULL; }  //  Not found.

            for (unsigned i = 0; i < ARRAY_SIZE(_instances); ++i) {
                SPI *inst = _instances[i];
                if (!inst) { continue; }
                if (!inst->mosi || !inst->miso || 
                    !inst->sclk || !inst->nss)
                    { continue; }

                if (inst->mosi->id == instance->mosi->id &&
                    inst->miso->id == instance->miso->id &&
                    inst->sclk->id == instance->sclk->id &&
                    inst->nss->id == instance->nss->id) 
                    { return inst; }  //  Found instance.
            }
            return NULL;  //  Not found.
        }

        static int enable_clock(uint32_t instance) {
#ifdef TODO
            switch (instance)
            {
            case SPI1_BASE:
                __HAL_RCC_SPI1_CLK_ENABLE();
                NVIC_EnableIRQ(SPI1_IRQn);
                return HAL_RCC_GetPCLK2Freq();
            case SPI2_BASE:
                __HAL_RCC_SPI2_CLK_ENABLE();
                NVIC_EnableIRQ(SPI2_IRQn);
                return HAL_RCC_GetPCLK1Freq();
        #ifdef SPI3_BASE
            case SPI3_BASE:
                __HAL_RCC_SPI3_CLK_ENABLE();
                NVIC_EnableIRQ(SPI3_IRQn);
                return HAL_RCC_GetPCLK1Freq();
        #endif
        #ifdef SPI4_BASE
            case SPI4_BASE:
                __HAL_RCC_SPI4_CLK_ENABLE();
                NVIC_EnableIRQ(SPI4_IRQn);
                return HAL_RCC_GetPCLK2Freq();
        #endif
        #ifdef SPI5_BASE
            case SPI5_BASE:
                __HAL_RCC_SPI5_CLK_ENABLE();
                NVIC_EnableIRQ(SPI5_IRQn);
                return HAL_RCC_GetPCLK2Freq();
        #endif
        #ifdef SPI6_BASE
            case SPI6_BASE:
                __HAL_RCC_SPI6_CLK_ENABLE();
                NVIC_EnableIRQ(SPI6_IRQn);
                return HAL_RCC_GetPCLK2Freq();
        #endif

            default:
                CODAL_ASSERT(0);
                return 0;
            }
#endif  //  TODO
            return 0;
        }

#ifdef TODO
        extern "C" void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
            SPI::_complete((uint32_t)hspi->Instance);
        }
        extern "C" void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi) {
            SPI::_complete((uint32_t)hspi->Instance);
        }
        extern "C" void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi) {
            SPI::_complete((uint32_t)hspi->Instance);
        }
        #define DEFIRQ(nm, id)                                                                             \
            extern "C" void nm() { SPI::_irq(id); }
        DEFIRQ(SPI1_IRQHandler, SPI1_BASE)
        DEFIRQ(SPI2_IRQHandler, SPI2_BASE)
        #ifdef SPI3_BASE
        DEFIRQ(SPI3_IRQHandler, SPI3_BASE)
        #endif
        #ifdef SPI4_BASE
        DEFIRQ(SPI4_IRQHandler, SPI4_BASE)
        #endif
        #ifdef SPI5_BASE
        DEFIRQ(SPI5_IRQHandler, SPI5_BASE)
        #endif
        #ifdef SPI6_BASE
        DEFIRQ(SPI6_IRQHandler, SPI6_BASE)
        #endif
        static const uint32_t baudprescaler[] = //
            {SPI_BAUDRATEPRESCALER_2, 2,
            SPI_BAUDRATEPRESCALER_4, 4,
            SPI_BAUDRATEPRESCALER_8, 8,
            SPI_BAUDRATEPRESCALER_16, 16,
            SPI_BAUDRATEPRESCALER_32, 32,
            SPI_BAUDRATEPRESCALER_64, 64,
            SPI_BAUDRATEPRESCALER_128, 128,
            SPI_BAUDRATEPRESCALER_256, 256,
            0, 0};
#endif  //  TODO

    }  //  namespace _cm
} // namespace codal
