/*
 *  Copyright (C) 2017, Nayuta, Inc. All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed to the Apache Software Foundation (ASF) under one
 *  or more contributor license agreements.  See the NOTICE file
 *  distributed with this work for additional information
 *  regarding copyright ownership.  The ASF licenses this file
 *  to you under the Apache License, Version 2.0 (the
 *  "License"); you may not use this file except in compliance
 *  with the License.  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing,
 *  software distributed under the License is distributed on an
 *  "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 *  KIND, either express or implied.  See the License for the
 *  specific language governing permissions and limitations
 *  under the License.
 */
/** @file   monitoring.h
 *  @brief  monitoring header
 */
#ifndef MONITORING_H__
#define MONITORING_H__

//forward definition
struct ln_self_t;
typedef struct ln_self_t ln_self_t;


#ifdef __cplusplus
extern "C" {
#endif


/********************************************************************
 * prototypes
 ********************************************************************/

/** チャネル閉鎖監視スレッド開始
 *
 * @param[in]   pArg        未使用
 * @retval      未使用
 */
void *monitor_thread_start(void *pArg);


/** モニタループ停止
 *
 */
void monitor_stop(void);


/** チャネルありnodeへの自動接続停止設定
 *
 * @param[in]   bDisable        true:自動接続停止
 */
void monitor_disable_autoconn(bool bDisable);


/** 最新のfeerate_per_kw取得
 *
 * @return      bitcoind estimatesmartfeeから算出したfeerate_per_kw
 */
uint32_t monitoring_get_latest_feerate_kw(void);


/** feerate_per_kw設定
 * 次回のfeerate更新周期で使用するfeerate_per_kwを手動設定する。
 * 0を指定した場合、bitcoind estimatesmartfeeから自動計算する。
 *
 * @param[in]   FeeratePerKw    feerate_per_kw
 */
void monitor_set_feerate_per_kw(uint32_t FeeratePerKw);


/** Unilateral Close(自分が展開)
 *
 * @param[in,out]       self        チャネル情報
 * @param[in,out]       pDbParam    DB情報
 */
bool monitor_close_unilateral_local(ln_self_t *self, void *pDbParam);


#ifdef __cplusplus
}
#endif

#endif  //MONITORING_H__
