/*
    Appendix B: Funding Transaction Test Vectors
        を適当に見て回る。

    https://github.com/lightningnetwork/lightning-rfc/blob/master/03-transactions.md#appendix-b-funding-transaction-test-vectors
*/


////////////////////////////////////////////////////////////////////////
//FAKE関数

//FAKE_VALUE_FUNC(int, external_function, int);

////////////////////////////////////////////////////////////////////////

class ln_bolt3_b: public testing::Test {
protected:
    virtual void SetUp() {
    }

    virtual void TearDown() {
    }

    static ucoin_buf_t redeem_fund;
    static ucoin_util_keys_t    keys;

public:
    static void DumpBin(const uint8_t *pData, uint16_t Len)
    {
        for (uint16_t lp = 0; lp < Len; lp++) {
            printf("%02x", pData[lp]);
        }
        printf("\n");
    }
};

ucoin_buf_t ln_bolt3_b::redeem_fund;
ucoin_util_keys_t    ln_bolt3_b::keys;


////////////////////////////////////////////////////////////////////////

TEST_F(ln_bolt3_b, fuding1)
{
    ucoin_init(UCOIN_TESTNET, true);


    static const uint8_t LOCAL_FUNDING_PUBKEY[] = {
        0x02, 0x3d, 0xa0, 0x92, 0xf6, 0x98, 0x0e, 0x58,
        0xd2, 0xc0, 0x37, 0x17, 0x31, 0x80, 0xe9, 0xa4,
        0x65, 0x47, 0x60, 0x26, 0xee, 0x50, 0xf9, 0x66,
        0x95, 0x96, 0x3e, 0x8e, 0xfe, 0x43, 0x6f, 0x54,
        0xeb,
    };
    static const uint8_t REMOTE_FUNDING_PUBKEY[] = {
        0x03, 0x0e, 0x9f, 0x7b, 0x62, 0x3d, 0x2c, 0xcc,
        0x7c, 0x9b, 0xd4, 0x4d, 0x66, 0xd5, 0xce, 0x21,
        0xce, 0x50, 0x4c, 0x0a, 0xcf, 0x63, 0x85, 0xa1,
        0x32, 0xce, 0xc6, 0xd3, 0xc3, 0x9f, 0xa7, 0x11,
        0xc1,
    };
    const uint8_t FUNDING_WSCRIPT[] = {
        0x52, 0x21, 0x02, 0x3d, 0xa0, 0x92, 0xf6, 0x98,
        0x0e, 0x58, 0xd2, 0xc0, 0x37, 0x17, 0x31, 0x80,
        0xe9, 0xa4, 0x65, 0x47, 0x60, 0x26, 0xee, 0x50,
        0xf9, 0x66, 0x95, 0x96, 0x3e, 0x8e, 0xfe, 0x43,
        0x6f, 0x54, 0xeb, 0x21, 0x03, 0x0e, 0x9f, 0x7b,
        0x62, 0x3d, 0x2c, 0xcc, 0x7c, 0x9b, 0xd4, 0x4d,
        0x66, 0xd5, 0xce, 0x21, 0xce, 0x50, 0x4c, 0x0a,
        0xcf, 0x63, 0x85, 0xa1, 0x32, 0xce, 0xc6, 0xd3,
        0xc3, 0x9f, 0xa7, 0x11, 0xc1, 0x52, 0xae,
    };

    bool ret;
    ucoin_keys_sort_t sort;

    ret = ucoin_util_create2of2(&redeem_fund, &sort, LOCAL_FUNDING_PUBKEY, REMOTE_FUNDING_PUBKEY);
    ASSERT_TRUE(ret);
    ASSERT_EQ(UCOIN_KEYS_SORT_ASC, sort);
    ASSERT_EQ(0, memcmp(FUNDING_WSCRIPT, redeem_fund.buf, sizeof(FUNDING_WSCRIPT)));
    ASSERT_EQ(sizeof(FUNDING_WSCRIPT), redeem_fund.len);

    ucoin_buf_free(&redeem_fund);

    //逆順
    ret = ucoin_util_create2of2(&redeem_fund, &sort, REMOTE_FUNDING_PUBKEY, LOCAL_FUNDING_PUBKEY);
    ASSERT_TRUE(ret);
    ASSERT_EQ(UCOIN_KEYS_SORT_OTHER, sort);
    ASSERT_EQ(0, memcmp(FUNDING_WSCRIPT, redeem_fund.buf, sizeof(FUNDING_WSCRIPT)));
    ASSERT_EQ(sizeof(FUNDING_WSCRIPT), redeem_fund.len);
}


TEST_F(ln_bolt3_b, fuding2)
{
    const char WIF_PRIV[]= "cRCH7YNcarfvaiY1GWUKQrRGmoezvfAiqHtdRvxe16shzbd7LDMz";
    //const char WIF_PUB[] = "mm3aPLSv9fBrbS68JzurAMp4xGoddJ6pSf";

    const uint8_t PRIV[] = {
        0x6b, 0xd0, 0x78, 0x65, 0x0f, 0xce, 0xe8, 0x44,
        0x4e, 0x4e, 0x09, 0x82, 0x52, 0x27, 0xb8, 0x01,
        0xa1, 0xca, 0x92, 0x8d, 0xeb, 0xb7, 0x50, 0xeb,
        0x36, 0xe6, 0xd5, 0x61, 0x24, 0xbb, 0x20, 0xe8,
    };

    bool ret;
    ucoin_chain_t chain;

    ret = ucoin_util_wif2keys(&keys, &chain, WIF_PRIV);
    ASSERT_TRUE(ret);
    ASSERT_EQ(0, memcmp(PRIV, keys.priv, sizeof(PRIV)));
    ASSERT_EQ(UCOIN_TESTNET, chain);
}


TEST_F(ln_bolt3_b, fuding3)
{

    //INPUTはP2PKH 1つ
    //お釣りはINPUT同じPUBKEYHASHに返すが、P2WPKHにする
    const uint8_t IN_TXID[] = {
//        0xfd, 0x21, 0x05, 0x60, 0x76, 0x05, 0xd2, 0x30,
//        0x29, 0x94, 0xff, 0xea, 0x70, 0x3b, 0x09, 0xf6,
//        0x6b, 0x63, 0x51, 0x81, 0x6e, 0xe7, 0x37, 0xa9,
//        0x3e, 0x42, 0xa8, 0x41, 0xea, 0x20, 0xbb, 0xad,
        0xad, 0xbb, 0x20, 0xea, 0x41, 0xa8, 0x42, 0x3e,
        0xa9, 0x37, 0xe7, 0x6e, 0x81, 0x51, 0x63, 0x6b,
        0xf6, 0x09, 0x3b, 0x70, 0xea, 0xff, 0x94, 0x29,
        0x30, 0xd2, 0x05, 0x76, 0x60, 0x05, 0x21, 0xfd,
    };
    const uint32_t IN_TXID_INDEX = 0;
    const uint64_t IN_SATOSHI = 5000000000LL;
    const uint64_t FUND_SATOSHI = 10000000LL;
    const int FEERATE_PER_KW = 15000;


    bool ret;
    ucoin_tx_t tx = UCOIN_TX_INIT;
    ucoin_buf_t txbuf = UCOIN_BUF_INIT;

    //output
    //vout#0
    ucoin_sw_add_vout_p2wsh(&tx, FUND_SATOSHI, &redeem_fund);

    //vout#1
    //      feeを計算した後で額を決定する
    ucoin_sw_add_vout_p2wpkh_pub(&tx, 0, keys.pub);

    //input
    //vin#0
    ucoin_tx_add_vin(&tx, IN_TXID, IN_TXID_INDEX);


    //FEE計算
    //      txサイズに署名の中間サイズと公開鍵サイズを加えたサイズにする
    //          http://bitcoin.stackexchange.com/questions/1195/how-to-calculate-transaction-size-before-sending
    ret = ucoin_tx_create(&txbuf, &tx);
    ASSERT_TRUE(ret);
    // LEN+署名(72) + LEN+公開鍵(33)
    uint64_t fee = (txbuf.len + 1 + 72 + 1 + 33) * 4 * FEERATE_PER_KW / 1000;
    tx.vout[1].value = IN_SATOSHI - FUND_SATOSHI - fee;
    ucoin_buf_free(&txbuf);


    //署名
    ret = ucoin_util_sign_p2pkh(&tx, 0, &keys);
    ASSERT_TRUE(ret);
    //ucoin_print_tx(&tx);

    const uint8_t FUNDING_TX[] = {
        0x02, 0x00, 0x00, 0x00, 0x01, 0xad, 0xbb, 0x20,
        0xea, 0x41, 0xa8, 0x42, 0x3e, 0xa9, 0x37, 0xe7,
        0x6e, 0x81, 0x51, 0x63, 0x6b, 0xf6, 0x09, 0x3b,
        0x70, 0xea, 0xff, 0x94, 0x29, 0x30, 0xd2, 0x05,
        0x76, 0x60, 0x05, 0x21, 0xfd, 0x00, 0x00, 0x00,
        0x00, 0x6b, 0x48, 0x30, 0x45, 0x02, 0x21, 0x00,
        0x90, 0x58, 0x7b, 0x62, 0x01, 0xe1, 0x66, 0xad,
        0x6a, 0xf0, 0x22, 0x7d, 0x30, 0x36, 0xa9, 0x45,
        0x42, 0x23, 0xd4, 0x9a, 0x1f, 0x11, 0x83, 0x9c,
        0x1a, 0x36, 0x21, 0x84, 0x34, 0x0e, 0xf0, 0x24,
        0x02, 0x20, 0x57, 0x7f, 0x7c, 0xd5, 0xcc, 0xa7,
        0x87, 0x19, 0x40, 0x5c, 0xbf, 0x1d, 0xe7, 0x41,
        0x4a, 0xc0, 0x27, 0xf0, 0x23, 0x9e, 0xf6, 0xe2,
        0x14, 0xc9, 0x0f, 0xca, 0xab, 0x04, 0x54, 0xd8,
        0x4b, 0x3b, 0x01, 0x21, 0x03, 0x53, 0x5b, 0x32,
        0xd5, 0xeb, 0x0a, 0x6e, 0xd0, 0x98, 0x2a, 0x04,
        0x79, 0xbb, 0xad, 0xc9, 0x86, 0x8d, 0x98, 0x36,
        0xf6, 0xba, 0x94, 0xdd, 0x5a, 0x63, 0xbe, 0x16,
        0xd8, 0x75, 0x06, 0x91, 0x84, 0xff, 0xff, 0xff,
        0xff, 0x02, 0x80, 0x96, 0x98, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x22, 0x00, 0x20, 0xc0, 0x15, 0xc4,
        0xa6, 0xbe, 0x01, 0x0e, 0x21, 0x65, 0x70, 0x68,
        0xfc, 0x2e, 0x6a, 0x9d, 0x02, 0xb2, 0x7e, 0xbe,
        0x4d, 0x49, 0x0a, 0x25, 0x84, 0x6f, 0x72, 0x37,
        0xf1, 0x04, 0xd1, 0xa3, 0xcd, 0x20, 0x25, 0x6d,
        0x29, 0x01, 0x00, 0x00, 0x00, 0x16, 0x00, 0x14,
        0x3c, 0xa3, 0x3c, 0x2e, 0x44, 0x46, 0xf4, 0xa3,
        0x05, 0xf2, 0x3c, 0x80, 0xdf, 0x8a, 0xd1, 0xaf,
        0xdc, 0xf6, 0x52, 0xf9, 0x00, 0x00, 0x00, 0x00,
    };
    ret = ucoin_tx_create(&txbuf, &tx);
    ASSERT_TRUE(ret);
    ASSERT_EQ(0, memcmp(FUNDING_TX, txbuf.buf, sizeof(FUNDING_TX)));
    ASSERT_EQ(sizeof(FUNDING_TX), txbuf.len);
    ucoin_buf_free(&txbuf);

    uint8_t txid[UCOIN_SZ_TXID];
    ucoin_tx_txid(txid, &tx);

    const uint8_t TXID_FUND[] = {
        0xbe, 0xf6, 0x7e, 0x4e, 0x2f, 0xb9, 0xdd, 0xee,
        0xb3, 0x46, 0x19, 0x73, 0xcd, 0x4c, 0x62, 0xab,
        0xb3, 0x50, 0x50, 0xb1, 0xad, 0xd7, 0x72, 0x99,
        0x5b, 0x82, 0x0b, 0x58, 0x4a, 0x48, 0x84, 0x89,
    };
    ASSERT_EQ(0, memcmp(TXID_FUND, txid, UCOIN_SZ_TXID));

    ucoin_tx_free(&tx);
    ucoin_buf_free(&redeem_fund);

    ucoin_term();

    ASSERT_EQ(0, ucoin_dbg_malloc_cnt());
}
