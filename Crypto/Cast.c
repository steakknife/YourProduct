/* Deprecated/legacy */


// cast.cpp - written and placed in the public domain by Wei Dai and Leonard Janke
// based on Steve Reid's public domain cast.c

/* Adapted for YourProduct */

#include <memory.h>
#include "Common/Tcdefs.h"
#include "Common/Endian.h"
#include "Cast.h"

#define word32 unsigned __int32
#define byte unsigned __int8
#define GETBYTE(x, y) (unsigned int)(byte)((x)>>(8*(y)))

/* Macros to access 8-bit bytes out of a 32-bit word */
#define U8a(x) GETBYTE(x,3)
#define U8b(x) GETBYTE(x,2)
#define U8c(x) GETBYTE(x,1)
#define U8d(x) GETBYTE(x,0)

static word32 rotlVariable (word32 x, unsigned int y)
{
	return (word32)((x<<y) | (x>>(sizeof(word32)*8-y)));
}


// CAST S-boxes

static const word32 S[8][256] = {
{
	0x30FB40D4UL, 0x9FA0FF0BUL, 0x6BECCD2FUL, 0x3F258C7AUL,
	0x1E213F2FUL, 0x9C004DD3UL, 0x6003E540UL, 0xCF9FC949UL,
	0xBFD4AF27UL, 0x88BBBDB5UL, 0xE2034090UL, 0x98D09675UL,
	0x6E63A0E0UL, 0x15C361D2UL, 0xC2E7661DUL, 0x22D4FF8EUL,
	0x28683B6FUL, 0xC07FD059UL, 0xFF2379C8UL, 0x775F50E2UL,
	0x43C340D3UL, 0xDF2F8656UL, 0x887CA41AUL, 0xA2D2BD2DUL,
	0xA1C9E0D6UL, 0x346C4819UL, 0x61B76D87UL, 0x22540F2FUL,
	0x2ABE32E1UL, 0xAA54166BUL, 0x22568E3AUL, 0xA2D341D0UL,
	0x66DB40C8UL, 0xA784392FUL, 0x004DFF2FUL, 0x2DB9D2DEUL,
	0x97943FACUL, 0x4A97C1D8UL, 0x527644B7UL, 0xB5F437A7UL,
	0xB82CBAEFUL, 0xD751D159UL, 0x6FF7F0EDUL, 0x5A097A1FUL,
	0x827B68D0UL, 0x90ECF52EUL, 0x22B0C054UL, 0xBC8E5935UL,
	0x4B6D2F7FUL, 0x50BB64A2UL, 0xD2664910UL, 0xBEE5812DUL,
	0xB7332290UL, 0xE93B159FUL, 0xB48EE411UL, 0x4BFF345DUL,
	0xFD45C240UL, 0xAD31973FUL, 0xC4F6D02EUL, 0x55FC8165UL,
	0xD5B1CAADUL, 0xA1AC2DAEUL, 0xA2D4B76DUL, 0xC19B0C50UL,
	0x882240F2UL, 0x0C6E4F38UL, 0xA4E4BFD7UL, 0x4F5BA272UL,
	0x564C1D2FUL, 0xC59C5319UL, 0xB949E354UL, 0xB04669FEUL,
	0xB1B6AB8AUL, 0xC71358DDUL, 0x6385C545UL, 0x110F935DUL,
	0x57538AD5UL, 0x6A390493UL, 0xE63D37E0UL, 0x2A54F6B3UL,
	0x3A787D5FUL, 0x6276A0B5UL, 0x19A6FCDFUL, 0x7A42206AUL,
	0x29F9D4D5UL, 0xF61B1891UL, 0xBB72275EUL, 0xAA508167UL,
	0x38901091UL, 0xC6B505EBUL, 0x84C7CB8CUL, 0x2AD75A0FUL,
	0x874A1427UL, 0xA2D1936BUL, 0x2AD286AFUL, 0xAA56D291UL,
	0xD7894360UL, 0x425C750DUL, 0x93B39E26UL, 0x187184C9UL,
	0x6C00B32DUL, 0x73E2BB14UL, 0xA0BEBC3CUL, 0x54623779UL,
	0x64459EABUL, 0x3F328B82UL, 0x7718CF82UL, 0x59A2CEA6UL,
	0x04EE002EUL, 0x89FE78E6UL, 0x3FAB0950UL, 0x325FF6C2UL,
	0x81383F05UL, 0x6963C5C8UL, 0x76CB5AD6UL, 0xD49974C9UL,
	0xCA180DCFUL, 0x380782D5UL, 0xC7FA5CF6UL, 0x8AC31511UL,
	0x35E79E13UL, 0x47DA91D0UL, 0xF40F9086UL, 0xA7E2419EUL,
	0x31366241UL, 0x051EF495UL, 0xAA573B04UL, 0x4A805D8DUL,
	0x548300D0UL, 0x00322A3CUL, 0xBF64CDDFUL, 0xBA57A68EUL,
	0x75C6372BUL, 0x50AFD341UL, 0xA7C13275UL, 0x915A0BF5UL,
	0x6B54BFABUL, 0x2B0B1426UL, 0xAB4CC9D7UL, 0x449CCD82UL,
	0xF7FBF265UL, 0xAB85C5F3UL, 0x1B55DB94UL, 0xAAD4E324UL,
	0xCFA4BD3FUL, 0x2DEAA3E2UL, 0x9E204D02UL, 0xC8BD25ACUL,
	0xEADF55B3UL, 0xD5BD9E98UL, 0xE31231B2UL, 0x2AD5AD6CUL,
	0x954329DEUL, 0xADBE4528UL, 0xD8710F69UL, 0xAA51C90FUL,
	0xAA786BF6UL, 0x22513F1EUL, 0xAA51A79BUL, 0x2AD344CCUL,
	0x7B5A41F0UL, 0xD37CFBADUL, 0x1B069505UL, 0x41ECE491UL,
	0xB4C332E6UL, 0x032268D4UL, 0xC9600ACCUL, 0xCE387E6DUL,
	0xBF6BB16CUL, 0x6A70FB78UL, 0x0D03D9C9UL, 0xD4DF39DEUL,
	0xE01063DAUL, 0x4736F464UL, 0x5AD328D8UL, 0xB347CC96UL,
	0x75BB0FC3UL, 0x98511BFBUL, 0x4FFBCC35UL, 0xB58BCF6AUL,
	0xE11F0ABCUL, 0xBFC5FE4AUL, 0xA70AEC10UL, 0xAC39570AUL,
	0x3F04442FUL, 0x6188B153UL, 0xE0397A2EUL, 0x5727CB79UL,
	0x9CEB418FUL, 0x1CACD68DUL, 0x2AD37C96UL, 0x0175CB9DUL,
	0xC69DFF09UL, 0xC75B65F0UL, 0xD9DB40D8UL, 0xEC0E7779UL,
	0x4744EAD4UL, 0xB11C3274UL, 0xDD24CB9EUL, 0x7E1C54BDUL,
	0xF01144F9UL, 0xD2240EB1UL, 0x9675B3FDUL, 0xA3AC3755UL,
	0xD47C27AFUL, 0x51C85F4DUL, 0x56907596UL, 0xA5BB15E6UL,
	0x580304F0UL, 0xCA042CF1UL, 0x011A37EAUL, 0x8DBFAADBUL,
	0x35BA3E4AUL, 0x3526FFA0UL, 0xC37B4D09UL, 0xBC306ED9UL,
	0x98A52666UL, 0x5648F725UL, 0xFF5E569DUL, 0x0CED63D0UL,
	0x7C63B2CFUL, 0x700B45E1UL, 0xD5EA50F1UL, 0x85A92872UL,
	0xAF1FBDA7UL, 0xD4234870UL, 0xA7870BF3UL, 0x2D3B4D79UL,
	0x42E04198UL, 0x0CD0EDE7UL, 0x26470DB8UL, 0xF881814CUL,
	0x474D6AD7UL, 0x7C0C5E5CUL, 0xD1231959UL, 0x381B7298UL,
	0xF5D2F4DBUL, 0xAB838653UL, 0x6E2F1E23UL, 0x83719C9EUL,
	0xBD91E046UL, 0x9A56456EUL, 0xDC39200CUL, 0x20C8C571UL,
	0x962BDA1CUL, 0xE1E696FFUL, 0xB141AB08UL, 0x7CCA89B9UL,
	0x1A69E783UL, 0x02CC4843UL, 0xA2F7C579UL, 0x429EF47DUL,
	0x427B169CUL, 0x5AC9F049UL, 0xDD8F0F00UL, 0x5C8165BFUL
},

{
	0x1F201094UL, 0xEF0BA75BUL, 0x69E3CF7EUL, 0x393F4380UL,
	0xFE61CF7AUL, 0xEEC5207AUL, 0x55889C94UL, 0x72FC0651UL,
	0xADA7EF79UL, 0x4E1D7235UL, 0xD55A63CEUL, 0xDE0436BAUL,
	0x99C430EFUL, 0x5F0C0794UL, 0x18DCDB7DUL, 0xA1D6EFF3UL,
	0xA0B52F7BUL, 0x59E83605UL, 0xEE15B094UL, 0xE9FFD909UL,
	0xDC440086UL, 0xEF944459UL, 0xBA83CCB3UL, 0xE0C3CDFBUL,
	0xD1DA4181UL, 0x3B092AB1UL, 0xF997F1C1UL, 0xA5E6CF7BUL,
	0x01420DDBUL, 0xE4E7EF5BUL, 0x25A1FF41UL, 0xE180F806UL,
	0x1FC41080UL, 0x179BEE7AUL, 0xD37AC6A9UL, 0xFE5830A4UL,
	0x98DE8B7FUL, 0x77E83F4EUL, 0x79929269UL, 0x24FA9F7BUL,
	0xE113C85BUL, 0xACC40083UL, 0xD7503525UL, 0xF7EA615FUL,
	0x62143154UL, 0x0D554B63UL, 0x5D681121UL, 0xC866C359UL,
	0x3D63CF73UL, 0xCEE234C0UL, 0xD4D87E87UL, 0x5C672B21UL,
	0x071F6181UL, 0x39F7627FUL, 0x361E3084UL, 0xE4EB573BUL,
	0x602F64A4UL, 0xD63ACD9CUL, 0x1BBC4635UL, 0x9E81032DUL,
	0x2701F50CUL, 0x99847AB4UL, 0xA0E3DF79UL, 0xBA6CF38CUL,
	0x10843094UL, 0x2537A95EUL, 0xF46F6FFEUL, 0xA1FF3B1FUL,
	0x208CFB6AUL, 0x8F458C74UL, 0xD9E0A227UL, 0x4EC73A34UL,
	0xFC884F69UL, 0x3E4DE8DFUL, 0xEF0E0088UL, 0x3559648DUL,
	0x8A45388CUL, 0x1D804366UL, 0x721D9BFDUL, 0xA58684BBUL,
	0xE8256333UL, 0x844E8212UL, 0x128D8098UL, 0xFED33FB4UL,
	0xCE280AE1UL, 0x27E19BA5UL, 0xD5A6C252UL, 0xE49754BDUL,
	0xC5D655DDUL, 0xEB667064UL, 0x77840B4DUL, 0xA1B6A801UL,
	0x84DB26A9UL, 0xE0B56714UL, 0x21F043B7UL, 0xE5D05860UL,
	0x54F03084UL, 0x066FF472UL, 0xA31AA153UL, 0xDADC4755UL,
	0xB5625DBFUL, 0x68561BE6UL, 0x83CA6B94UL, 0x2D6ED23BUL,
	0xECCF01DBUL, 0xA6D3D0BAUL, 0xB6803D5CUL, 0xAF77A709UL,
	0x33B4A34CUL, 0x397BC8D6UL, 0x5EE22B95UL, 0x5F0E5304UL,
	0x81ED6F61UL, 0x20E74364UL, 0xB45E1378UL, 0xDE18639BUL,
	0x881CA122UL, 0xB96726D1UL, 0x8049A7E8UL, 0x22B7DA7BUL,
	0x5E552D25UL, 0x5272D237UL, 0x79D2951CUL, 0xC60D894CUL,
	0x488CB402UL, 0x1BA4FE5BUL, 0xA4B09F6BUL, 0x1CA815CFUL,
	0xA20C3005UL, 0x8871DF63UL, 0xB9DE2FCBUL, 0x0CC6C9E9UL,
	0x0BEEFF53UL, 0xE3214517UL, 0xB4542835UL, 0x9F63293CUL,
	0xEE41E729UL, 0x6E1D2D7CUL, 0x50045286UL, 0x1E6685F3UL,
	0xF33401C6UL, 0x30A22C95UL, 0x31A70850UL, 0x60930F13UL,
	0x73F98417UL, 0xA1269859UL, 0xEC645C44UL, 0x52C877A9UL,
	0xCDFF33A6UL, 0xA02B1741UL, 0x7CBAD9A2UL, 0x2180036FUL,
	0x50D99C08UL, 0xCB3F4861UL, 0xC26BD765UL, 0x64A3F6ABUL,
	0x80342676UL, 0x25A75E7BUL, 0xE4E6D1FCUL, 0x20C710E6UL,
	0xCDF0B680UL, 0x17844D3BUL, 0x31EEF84DUL, 0x7E0824E4UL,
	0x2CCB49EBUL, 0x846A3BAEUL, 0x8FF77888UL, 0xEE5D60F6UL,
	0x7AF75673UL, 0x2FDD5CDBUL, 0xA11631C1UL, 0x30F66F43UL,
	0xB3FAEC54UL, 0x157FD7FAUL, 0xEF8579CCUL, 0xD152DE58UL,
	0xDB2FFD5EUL, 0x8F32CE19UL, 0x306AF97AUL, 0x02F03EF8UL,
	0x99319AD5UL, 0xC242FA0FUL, 0xA7E3EBB0UL, 0xC68E4906UL,
	0xB8DA230CUL, 0x80823028UL, 0xDCDEF3C8UL, 0xD35FB171UL,
	0x088A1BC8UL, 0xBEC0C560UL, 0x61A3C9E8UL, 0xBCA8F54DUL,
	0xC72FEFFAUL, 0x22822E99UL, 0x82C570B4UL, 0xD8D94E89UL,
	0x8B1C34BCUL, 0x301E16E6UL, 0x273BE979UL, 0xB0FFEAA6UL,
	0x61D9B8C6UL, 0x00B24869UL, 0xB7FFCE3FUL, 0x08DC283BUL,
	0x43DAF65AUL, 0xF7E19798UL, 0x7619B72FUL, 0x8F1C9BA4UL,
	0xDC8637A0UL, 0x16A7D3B1UL, 0x9FC393B7UL, 0xA7136EEBUL,
	0xC6BCC63EUL, 0x1A513742UL, 0xEF6828BCUL, 0x520365D6UL,
	0x2D6A77ABUL, 0x3527ED4BUL, 0x821FD216UL, 0x095C6E2EUL,
	0xDB92F2FBUL, 0x5EEA29CBUL, 0x145892F5UL, 0x91584F7FUL,
	0x5483697BUL, 0x2667A8CCUL, 0x85196048UL, 0x8C4BACEAUL,
	0x833860D4UL, 0x0D23E0F9UL, 0x6C387E8AUL, 0x0AE6D249UL,
	0xB284600CUL, 0xD835731DUL, 0xDCB1C647UL, 0xAC4C56EAUL,
	0x3EBD81B3UL, 0x230EABB0UL, 0x6438BC87UL, 0xF0B5B1FAUL,
	0x8F5EA2B3UL, 0xFC184642UL, 0x0A036B7AUL, 0x4FB089BDUL,
	0x649DA589UL, 0xA345415EUL, 0x5C038323UL, 0x3E5D3BB9UL,
	0x43D79572UL, 0x7E6DD07CUL, 0x06DFDF1EUL, 0x6C6CC4EFUL,
	0x7160A539UL, 0x73BFBE70UL, 0x83877605UL, 0x4523ECF1UL
},

{
	0x8DEFC240UL, 0x25FA5D9FUL, 0xEB903DBFUL, 0xE810C907UL,
	0x47607FFFUL, 0x369FE44BUL, 0x8C1FC644UL, 0xAECECA90UL,
	0xBEB1F9BFUL, 0xEEFBCAEAUL, 0xE8CF1950UL, 0x51DF07AEUL,
	0x920E8806UL, 0xF0AD0548UL, 0xE13C8D83UL, 0x927010D5UL,
	0x11107D9FUL, 0x07647DB9UL, 0xB2E3E4D4UL, 0x3D4F285EUL,
	0xB9AFA820UL, 0xFADE82E0UL, 0xA067268BUL, 0x8272792EUL,
	0x553FB2C0UL, 0x489AE22BUL, 0xD4EF9794UL, 0x125E3FBCUL,
	0x21FFFCEEUL, 0x825B1BFDUL, 0x9255C5EDUL, 0x1257A240UL,
	0x4E1A8302UL, 0xBAE07FFFUL, 0x528246E7UL, 0x8E57140EUL,
	0x3373F7BFUL, 0x8C9F8188UL, 0xA6FC4EE8UL, 0xC982B5A5UL,
	0xA8C01DB7UL, 0x579FC264UL, 0x67094F31UL, 0xF2BD3F5FUL,
	0x40FFF7C1UL, 0x1FB78DFCUL, 0x8E6BD2C1UL, 0x437BE59BUL,
	0x99B03DBFUL, 0xB5DBC64BUL, 0x638DC0E6UL, 0x55819D99UL,
	0xA197C81CUL, 0x4A012D6EUL, 0xC5884A28UL, 0xCCC36F71UL,
	0xB843C213UL, 0x6C0743F1UL, 0x8309893CUL, 0x0FEDDD5FUL,
	0x2F7FE850UL, 0xD7C07F7EUL, 0x02507FBFUL, 0x5AFB9A04UL,
	0xA747D2D0UL, 0x1651192EUL, 0xAF70BF3EUL, 0x58C31380UL,
	0x5F98302EUL, 0x727CC3C4UL, 0x0A0FB402UL, 0x0F7FEF82UL,
	0x8C96FDADUL, 0x5D2C2AAEUL, 0x8EE99A49UL, 0x50DA88B8UL,
	0x8427F4A0UL, 0x1EAC5790UL, 0x796FB449UL, 0x8252DC15UL,
	0xEFBD7D9BUL, 0xA672597DUL, 0xADA840D8UL, 0x45F54504UL,
	0xFA5D7403UL, 0xE83EC305UL, 0x4F91751AUL, 0x925669C2UL,
	0x23EFE941UL, 0xA903F12EUL, 0x60270DF2UL, 0x0276E4B6UL,
	0x94FD6574UL, 0x927985B2UL, 0x8276DBCBUL, 0x02778176UL,
	0xF8AF918DUL, 0x4E48F79EUL, 0x8F616DDFUL, 0xE29D840EUL,
	0x842F7D83UL, 0x340CE5C8UL, 0x96BBB682UL, 0x93B4B148UL,
	0xEF303CABUL, 0x984FAF28UL, 0x779FAF9BUL, 0x92DC560DUL,
	0x224D1E20UL, 0x8437AA88UL, 0x7D29DC96UL, 0x2756D3DCUL,
	0x8B907CEEUL, 0xB51FD240UL, 0xE7C07CE3UL, 0xE566B4A1UL,
	0xC3E9615EUL, 0x3CF8209DUL, 0x6094D1E3UL, 0xCD9CA341UL,
	0x5C76460EUL, 0x00EA983BUL, 0xD4D67881UL, 0xFD47572CUL,
	0xF76CEDD9UL, 0xBDA8229CUL, 0x127DADAAUL, 0x438A074EUL,
	0x1F97C090UL, 0x081BDB8AUL, 0x93A07EBEUL, 0xB938CA15UL,
	0x97B03CFFUL, 0x3DC2C0F8UL, 0x8D1AB2ECUL, 0x64380E51UL,
	0x68CC7BFBUL, 0xD90F2788UL, 0x12490181UL, 0x5DE5FFD4UL,
	0xDD7EF86AUL, 0x76A2E214UL, 0xB9A40368UL, 0x925D958FUL,
	0x4B39FFFAUL, 0xBA39AEE9UL, 0xA4FFD30BUL, 0xFAF7933BUL,
	0x6D498623UL, 0x193CBCFAUL, 0x27627545UL, 0x825CF47AUL,
	0x61BD8BA0UL, 0xD11E42D1UL, 0xCEAD04F4UL, 0x127EA392UL,
	0x10428DB7UL, 0x8272A972UL, 0x9270C4A8UL, 0x127DE50BUL,
	0x285BA1C8UL, 0x3C62F44FUL, 0x35C0EAA5UL, 0xE805D231UL,
	0x428929FBUL, 0xB4FCDF82UL, 0x4FB66A53UL, 0x0E7DC15BUL,
	0x1F081FABUL, 0x108618AEUL, 0xFCFD086DUL, 0xF9FF2889UL,
	0x694BCC11UL, 0x236A5CAEUL, 0x12DECA4DUL, 0x2C3F8CC5UL,
	0xD2D02DFEUL, 0xF8EF5896UL, 0xE4CF52DAUL, 0x95155B67UL,
	0x494A488CUL, 0xB9B6A80CUL, 0x5C8F82BCUL, 0x89D36B45UL,
	0x3A609437UL, 0xEC00C9A9UL, 0x44715253UL, 0x0A874B49UL,
	0xD773BC40UL, 0x7C34671CUL, 0x02717EF6UL, 0x4FEB5536UL,
	0xA2D02FFFUL, 0xD2BF60C4UL, 0xD43F03C0UL, 0x50B4EF6DUL,
	0x07478CD1UL, 0x006E1888UL, 0xA2E53F55UL, 0xB9E6D4BCUL,
	0xA2048016UL, 0x97573833UL, 0xD7207D67UL, 0xDE0F8F3DUL,
	0x72F87B33UL, 0xABCC4F33UL, 0x7688C55DUL, 0x7B00A6B0UL,
	0x947B0001UL, 0x570075D2UL, 0xF9BB88F8UL, 0x8942019EUL,
	0x4264A5FFUL, 0x856302E0UL, 0x72DBD92BUL, 0xEE971B69UL,
	0x6EA22FDEUL, 0x5F08AE2BUL, 0xAF7A616DUL, 0xE5C98767UL,
	0xCF1FEBD2UL, 0x61EFC8C2UL, 0xF1AC2571UL, 0xCC8239C2UL,
	0x67214CB8UL, 0xB1E583D1UL, 0xB7DC3E62UL, 0x7F10BDCEUL,
	0xF90A5C38UL, 0x0FF0443DUL, 0x606E6DC6UL, 0x60543A49UL,
	0x5727C148UL, 0x2BE98A1DUL, 0x8AB41738UL, 0x20E1BE24UL,
	0xAF96DA0FUL, 0x68458425UL, 0x99833BE5UL, 0x600D457DUL,
	0x282F9350UL, 0x8334B362UL, 0xD91D1120UL, 0x2B6D8DA0UL,
	0x642B1E31UL, 0x9C305A00UL, 0x52BCE688UL, 0x1B03588AUL,
	0xF7BAEFD5UL, 0x4142ED9CUL, 0xA4315C11UL, 0x83323EC5UL,
	0xDFEF4636UL, 0xA133C501UL, 0xE9D3531CUL, 0xEE353783UL
},

{
	0x9DB30420UL, 0x1FB6E9DEUL, 0xA7BE7BEFUL, 0xD273A298UL,
	0x4A4F7BDBUL, 0x64AD8C57UL, 0x85510443UL, 0xFA020ED1UL,
	0x7E287AFFUL, 0xE60FB663UL, 0x095F35A1UL, 0x79EBF120UL,
	0xFD059D43UL, 0x6497B7B1UL, 0xF3641F63UL, 0x241E4ADFUL,
	0x28147F5FUL, 0x4FA2B8CDUL, 0xC9430040UL, 0x0CC32220UL,
	0xFDD30B30UL, 0xC0A5374FUL, 0x1D2D00D9UL, 0x24147B15UL,
	0xEE4D111AUL, 0x0FCA5167UL, 0x71FF904CUL, 0x2D195FFEUL,
	0x1A05645FUL, 0x0C13FEFEUL, 0x081B08CAUL, 0x05170121UL,
	0x80530100UL, 0xE83E5EFEUL, 0xAC9AF4F8UL, 0x7FE72701UL,
	0xD2B8EE5FUL, 0x06DF4261UL, 0xBB9E9B8AUL, 0x7293EA25UL,
	0xCE84FFDFUL, 0xF5718801UL, 0x3DD64B04UL, 0xA26F263BUL,
	0x7ED48400UL, 0x547EEBE6UL, 0x446D4CA0UL, 0x6CF3D6F5UL,
	0x2649ABDFUL, 0xAEA0C7F5UL, 0x36338CC1UL, 0x503F7E93UL,
	0xD3772061UL, 0x11B638E1UL, 0x72500E03UL, 0xF80EB2BBUL,
	0xABE0502EUL, 0xEC8D77DEUL, 0x57971E81UL, 0xE14F6746UL,
	0xC9335400UL, 0x6920318FUL, 0x081DBB99UL, 0xFFC304A5UL,
	0x4D351805UL, 0x7F3D5CE3UL, 0xA6C866C6UL, 0x5D5BCCA9UL,
	0xDAEC6FEAUL, 0x9F926F91UL, 0x9F46222FUL, 0x3991467DUL,
	0xA5BF6D8EUL, 0x1143C44FUL, 0x43958302UL, 0xD0214EEBUL,
	0x022083B8UL, 0x3FB6180CUL, 0x18F8931EUL, 0x281658E6UL,
	0x26486E3EUL, 0x8BD78A70UL, 0x7477E4C1UL, 0xB506E07CUL,
	0xF32D0A25UL, 0x79098B02UL, 0xE4EABB81UL, 0x28123B23UL,
	0x69DEAD38UL, 0x1574CA16UL, 0xDF871B62UL, 0x211C40B7UL,
	0xA51A9EF9UL, 0x0014377BUL, 0x041E8AC8UL, 0x09114003UL,
	0xBD59E4D2UL, 0xE3D156D5UL, 0x4FE876D5UL, 0x2F91A340UL,
	0x557BE8DEUL, 0x00EAE4A7UL, 0x0CE5C2ECUL, 0x4DB4BBA6UL,
	0xE756BDFFUL, 0xDD3369ACUL, 0xEC17B035UL, 0x06572327UL,
	0x99AFC8B0UL, 0x56C8C391UL, 0x6B65811CUL, 0x5E146119UL,
	0x6E85CB75UL, 0xBE07C002UL, 0xC2325577UL, 0x893FF4ECUL,
	0x5BBFC92DUL, 0xD0EC3B25UL, 0xB7801AB7UL, 0x8D6D3B24UL,
	0x20C763EFUL, 0xC366A5FCUL, 0x9C382880UL, 0x0ACE3205UL,
	0xAAC9548AUL, 0xECA1D7C7UL, 0x041AFA32UL, 0x1D16625AUL,
	0x6701902CUL, 0x9B757A54UL, 0x31D477F7UL, 0x9126B031UL,
	0x36CC6FDBUL, 0xC70B8B46UL, 0xD9E66A48UL, 0x56E55A79UL,
	0x026A4CEBUL, 0x52437EFFUL, 0x2F8F76B4UL, 0x0DF980A5UL,
	0x8674CDE3UL, 0xEDDA04EBUL, 0x17A9BE04UL, 0x2C18F4DFUL,
	0xB7747F9DUL, 0xAB2AF7B4UL, 0xEFC34D20UL, 0x2E096B7CUL,
	0x1741A254UL, 0xE5B6A035UL, 0x213D42F6UL, 0x2C1C7C26UL,
	0x61C2F50FUL, 0x6552DAF9UL, 0xD2C231F8UL, 0x25130F69UL,
	0xD8167FA2UL, 0x0418F2C8UL, 0x001A96A6UL, 0x0D1526ABUL,
	0x63315C21UL, 0x5E0A72ECUL, 0x49BAFEFDUL, 0x187908D9UL,
	0x8D0DBD86UL, 0x311170A7UL, 0x3E9B640CUL, 0xCC3E10D7UL,
	0xD5CAD3B6UL, 0x0CAEC388UL, 0xF73001E1UL, 0x6C728AFFUL,
	0x71EAE2A1UL, 0x1F9AF36EUL, 0xCFCBD12FUL, 0xC1DE8417UL,
	0xAC07BE6BUL, 0xCB44A1D8UL, 0x8B9B0F56UL, 0x013988C3UL,
	0xB1C52FCAUL, 0xB4BE31CDUL, 0xD8782806UL, 0x12A3A4E2UL,
	0x6F7DE532UL, 0x58FD7EB6UL, 0xD01EE900UL, 0x24ADFFC2UL,
	0xF4990FC5UL, 0x9711AAC5UL, 0x001D7B95UL, 0x82E5E7D2UL,
	0x109873F6UL, 0x00613096UL, 0xC32D9521UL, 0xADA121FFUL,
	0x29908415UL, 0x7FBB977FUL, 0xAF9EB3DBUL, 0x29C9ED2AUL,
	0x5CE2A465UL, 0xA730F32CUL, 0xD0AA3FE8UL, 0x8A5CC091UL,
	0xD49E2CE7UL, 0x0CE454A9UL, 0xD60ACD86UL, 0x015F1919UL,
	0x77079103UL, 0xDEA03AF6UL, 0x78A8565EUL, 0xDEE356DFUL,
	0x21F05CBEUL, 0x8B75E387UL, 0xB3C50651UL, 0xB8A5C3EFUL,
	0xD8EEB6D2UL, 0xE523BE77UL, 0xC2154529UL, 0x2F69EFDFUL,
	0xAFE67AFBUL, 0xF470C4B2UL, 0xF3E0EB5BUL, 0xD6CC9876UL,
	0x39E4460CUL, 0x1FDA8538UL, 0x1987832FUL, 0xCA007367UL,
	0xA99144F8UL, 0x296B299EUL, 0x492FC295UL, 0x9266BEABUL,
	0xB5676E69UL, 0x9BD3DDDAUL, 0xDF7E052FUL, 0xDB25701CUL,
	0x1B5E51EEUL, 0xF65324E6UL, 0x6AFCE36CUL, 0x0316CC04UL,
	0x8644213EUL, 0xB7DC59D0UL, 0x7965291FUL, 0xCCD6FD43UL,
	0x41823979UL, 0x932BCDF6UL, 0xB657C34DUL, 0x4EDFD282UL,
	0x7AE5290CUL, 0x3CB9536BUL, 0x851E20FEUL, 0x9833557EUL,
	0x13ECF0B0UL, 0xD3FFB372UL, 0x3F85C5C1UL, 0x0AEF7ED2UL
},

{
	0x7EC90C04UL, 0x2C6E74B9UL, 0x9B0E66DFUL, 0xA6337911UL,
	0xB86A7FFFUL, 0x1DD358F5UL, 0x44DD9D44UL, 0x1731167FUL,
	0x08FBF1FAUL, 0xE7F511CCUL, 0xD2051B00UL, 0x735ABA00UL,
	0x2AB722D8UL, 0x386381CBUL, 0xACF6243AUL, 0x69BEFD7AUL,
	0xE6A2E77FUL, 0xF0C720CDUL, 0xC4494816UL, 0xCCF5C180UL,
	0x38851640UL, 0x15B0A848UL, 0xE68B18CBUL, 0x4CAADEFFUL,
	0x5F480A01UL, 0x0412B2AAUL, 0x259814FCUL, 0x41D0EFE2UL,
	0x4E40B48DUL, 0x248EB6FBUL, 0x8DBA1CFEUL, 0x41A99B02UL,
	0x1A550A04UL, 0xBA8F65CBUL, 0x7251F4E7UL, 0x95A51725UL,
	0xC106ECD7UL, 0x97A5980AUL, 0xC539B9AAUL, 0x4D79FE6AUL,
	0xF2F3F763UL, 0x68AF8040UL, 0xED0C9E56UL, 0x11B4958BUL,
	0xE1EB5A88UL, 0x8709E6B0UL, 0xD7E07156UL, 0x4E29FEA7UL,
	0x6366E52DUL, 0x02D1C000UL, 0xC4AC8E05UL, 0x9377F571UL,
	0x0C05372AUL, 0x578535F2UL, 0x2261BE02UL, 0xD642A0C9UL,
	0xDF13A280UL, 0x74B55BD2UL, 0x682199C0UL, 0xD421E5ECUL,
	0x53FB3CE8UL, 0xC8ADEDB3UL, 0x28A87FC9UL, 0x3D959981UL,
	0x5C1FF900UL, 0xFE38D399UL, 0x0C4EFF0BUL, 0x062407EAUL,
	0xAA2F4FB1UL, 0x4FB96976UL, 0x90C79505UL, 0xB0A8A774UL,
	0xEF55A1FFUL, 0xE59CA2C2UL, 0xA6B62D27UL, 0xE66A4263UL,
	0xDF65001FUL, 0x0EC50966UL, 0xDFDD55BCUL, 0x29DE0655UL,
	0x911E739AUL, 0x17AF8975UL, 0x32C7911CUL, 0x89F89468UL,
	0x0D01E980UL, 0x524755F4UL, 0x03B63CC9UL, 0x0CC844B2UL,
	0xBCF3F0AAUL, 0x87AC36E9UL, 0xE53A7426UL, 0x01B3D82BUL,
	0x1A9E7449UL, 0x64EE2D7EUL, 0xCDDBB1DAUL, 0x01C94910UL,
	0xB868BF80UL, 0x0D26F3FDUL, 0x9342EDE7UL, 0x04A5C284UL,
	0x636737B6UL, 0x50F5B616UL, 0xF24766E3UL, 0x8ECA36C1UL,
	0x136E05DBUL, 0xFEF18391UL, 0xFB887A37UL, 0xD6E7F7D4UL,
	0xC7FB7DC9UL, 0x3063FCDFUL, 0xB6F589DEUL, 0xEC2941DAUL,
	0x26E46695UL, 0xB7566419UL, 0xF654EFC5UL, 0xD08D58B7UL,
	0x48925401UL, 0xC1BACB7FUL, 0xE5FF550FUL, 0xB6083049UL,
	0x5BB5D0E8UL, 0x87D72E5AUL, 0xAB6A6EE1UL, 0x223A66CEUL,
	0xC62BF3CDUL, 0x9E0885F9UL, 0x68CB3E47UL, 0x086C010FUL,
	0xA21DE820UL, 0xD18B69DEUL, 0xF3F65777UL, 0xFA02C3F6UL,
	0x407EDAC3UL, 0xCBB3D550UL, 0x1793084DUL, 0xB0D70EBAUL,
	0x0AB378D5UL, 0xD951FB0CUL, 0xDED7DA56UL, 0x4124BBE4UL,
	0x94CA0B56UL, 0x0F5755D1UL, 0xE0E1E56EUL, 0x6184B5BEUL,
	0x580A249FUL, 0x94F74BC0UL, 0xE327888EUL, 0x9F7B5561UL,
	0xC3DC0280UL, 0x05687715UL, 0x646C6BD7UL, 0x44904DB3UL,
	0x66B4F0A3UL, 0xC0F1648AUL, 0x697ED5AFUL, 0x49E92FF6UL,
	0x309E374FUL, 0x2CB6356AUL, 0x85808573UL, 0x4991F840UL,
	0x76F0AE02UL, 0x083BE84DUL, 0x28421C9AUL, 0x44489406UL,
	0x736E4CB8UL, 0xC1092910UL, 0x8BC95FC6UL, 0x7D869CF4UL,
	0x134F616FUL, 0x2E77118DUL, 0xB31B2BE1UL, 0xAA90B472UL,
	0x3CA5D717UL, 0x7D161BBAUL, 0x9CAD9010UL, 0xAF462BA2UL,
	0x9FE459D2UL, 0x45D34559UL, 0xD9F2DA13UL, 0xDBC65487UL,
	0xF3E4F94EUL, 0x176D486FUL, 0x097C13EAUL, 0x631DA5C7UL,
	0x445F7382UL, 0x175683F4UL, 0xCDC66A97UL, 0x70BE0288UL,
	0xB3CDCF72UL, 0x6E5DD2F3UL, 0x20936079UL, 0x459B80A5UL,
	0xBE60E2DBUL, 0xA9C23101UL, 0xEBA5315CUL, 0x224E42F2UL,
	0x1C5C1572UL, 0xF6721B2CUL, 0x1AD2FFF3UL, 0x8C25404EUL,
	0x324ED72FUL, 0x4067B7FDUL, 0x0523138EUL, 0x5CA3BC78UL,
	0xDC0FD66EUL, 0x75922283UL, 0x784D6B17UL, 0x58EBB16EUL,
	0x44094F85UL, 0x3F481D87UL, 0xFCFEAE7BUL, 0x77B5FF76UL,
	0x8C2302BFUL, 0xAAF47556UL, 0x5F46B02AUL, 0x2B092801UL,
	0x3D38F5F7UL, 0x0CA81F36UL, 0x52AF4A8AUL, 0x66D5E7C0UL,
	0xDF3B0874UL, 0x95055110UL, 0x1B5AD7A8UL, 0xF61ED5ADUL,
	0x6CF6E479UL, 0x20758184UL, 0xD0CEFA65UL, 0x88F7BE58UL,
	0x4A046826UL, 0x0FF6F8F3UL, 0xA09C7F70UL, 0x5346ABA0UL,
	0x5CE96C28UL, 0xE176EDA3UL, 0x6BAC307FUL, 0x376829D2UL,
	0x85360FA9UL, 0x17E3FE2AUL, 0x24B79767UL, 0xF5A96B20UL,
	0xD6CD2595UL, 0x68FF1EBFUL, 0x7555442CUL, 0xF19F06BEUL,
	0xF9E0659AUL, 0xEEB9491DUL, 0x34010718UL, 0xBB30CAB8UL,
	0xE822FE15UL, 0x88570983UL, 0x750E6249UL, 0xDA627E55UL,
	0x5E76FFA8UL, 0xB1534546UL, 0x6D47DE08UL, 0xEFE9E7D4UL
},

{
	0xF6FA8F9DUL, 0x2CAC6CE1UL, 0x4CA34867UL, 0xE2337F7CUL,
	0x95DB08E7UL, 0x016843B4UL, 0xECED5CBCUL, 0x325553ACUL,
	0xBF9F0960UL, 0xDFA1E2EDUL, 0x83F0579DUL, 0x63ED86B9UL,
	0x1AB6A6B8UL, 0xDE5EBE39UL, 0xF38FF732UL, 0x8989B138UL,
	0x33F14961UL, 0xC01937BDUL, 0xF506C6DAUL, 0xE4625E7EUL,
	0xA308EA99UL, 0x4E23E33CUL, 0x79CBD7CCUL, 0x48A14367UL,
	0xA3149619UL, 0xFEC94BD5UL, 0xA114174AUL, 0xEAA01866UL,
	0xA084DB2DUL, 0x09A8486FUL, 0xA888614AUL, 0x2900AF98UL,
	0x01665991UL, 0xE1992863UL, 0xC8F30C60UL, 0x2E78EF3CUL,
	0xD0D51932UL, 0xCF0FEC14UL, 0xF7CA07D2UL, 0xD0A82072UL,
	0xFD41197EUL, 0x9305A6B0UL, 0xE86BE3DAUL, 0x74BED3CDUL,
	0x372DA53CUL, 0x4C7F4448UL, 0xDAB5D440UL, 0x6DBA0EC3UL,
	0x083919A7UL, 0x9FBAEED9UL, 0x49DBCFB0UL, 0x4E670C53UL,
	0x5C3D9C01UL, 0x64BDB941UL, 0x2C0E636AUL, 0xBA7DD9CDUL,
	0xEA6F7388UL, 0xE70BC762UL, 0x35F29ADBUL, 0x5C4CDD8DUL,
	0xF0D48D8CUL, 0xB88153E2UL, 0x08A19866UL, 0x1AE2EAC8UL,
	0x284CAF89UL, 0xAA928223UL, 0x9334BE53UL, 0x3B3A21BFUL,
	0x16434BE3UL, 0x9AEA3906UL, 0xEFE8C36EUL, 0xF890CDD9UL,
	0x80226DAEUL, 0xC340A4A3UL, 0xDF7E9C09UL, 0xA694A807UL,
	0x5B7C5ECCUL, 0x221DB3A6UL, 0x9A69A02FUL, 0x68818A54UL,
	0xCEB2296FUL, 0x53C0843AUL, 0xFE893655UL, 0x25BFE68AUL,
	0xB4628ABCUL, 0xCF222EBFUL, 0x25AC6F48UL, 0xA9A99387UL,
	0x53BDDB65UL, 0xE76FFBE7UL, 0xE967FD78UL, 0x0BA93563UL,
	0x8E342BC1UL, 0xE8A11BE9UL, 0x4980740DUL, 0xC8087DFCUL,
	0x8DE4BF99UL, 0xA11101A0UL, 0x7FD37975UL, 0xDA5A26C0UL,
	0xE81F994FUL, 0x9528CD89UL, 0xFD339FEDUL, 0xB87834BFUL,
	0x5F04456DUL, 0x22258698UL, 0xC9C4C83BUL, 0x2DC156BEUL,
	0x4F628DAAUL, 0x57F55EC5UL, 0xE2220ABEUL, 0xD2916EBFUL,
	0x4EC75B95UL, 0x24F2C3C0UL, 0x42D15D99UL, 0xCD0D7FA0UL,
	0x7B6E27FFUL, 0xA8DC8AF0UL, 0x7345C106UL, 0xF41E232FUL,
	0x35162386UL, 0xE6EA8926UL, 0x3333B094UL, 0x157EC6F2UL,
	0x372B74AFUL, 0x692573E4UL, 0xE9A9D848UL, 0xF3160289UL,
	0x3A62EF1DUL, 0xA787E238UL, 0xF3A5F676UL, 0x74364853UL,
	0x20951063UL, 0x4576698DUL, 0xB6FAD407UL, 0x592AF950UL,
	0x36F73523UL, 0x4CFB6E87UL, 0x7DA4CEC0UL, 0x6C152DAAUL,
	0xCB0396A8UL, 0xC50DFE5DUL, 0xFCD707ABUL, 0x0921C42FUL,
	0x89DFF0BBUL, 0x5FE2BE78UL, 0x448F4F33UL, 0x754613C9UL,
	0x2B05D08DUL, 0x48B9D585UL, 0xDC049441UL, 0xC8098F9BUL,
	0x7DEDE786UL, 0xC39A3373UL, 0x42410005UL, 0x6A091751UL,
	0x0EF3C8A6UL, 0x890072D6UL, 0x28207682UL, 0xA9A9F7BEUL,
	0xBF32679DUL, 0xD45B5B75UL, 0xB353FD00UL, 0xCBB0E358UL,
	0x830F220AUL, 0x1F8FB214UL, 0xD372CF08UL, 0xCC3C4A13UL,
	0x8CF63166UL, 0x061C87BEUL, 0x88C98F88UL, 0x6062E397UL,
	0x47CF8E7AUL, 0xB6C85283UL, 0x3CC2ACFBUL, 0x3FC06976UL,
	0x4E8F0252UL, 0x64D8314DUL, 0xDA3870E3UL, 0x1E665459UL,
	0xC10908F0UL, 0x513021A5UL, 0x6C5B68B7UL, 0x822F8AA0UL,
	0x3007CD3EUL, 0x74719EEFUL, 0xDC872681UL, 0x073340D4UL,
	0x7E432FD9UL, 0x0C5EC241UL, 0x8809286CUL, 0xF592D891UL,
	0x08A930F6UL, 0x957EF305UL, 0xB7FBFFBDUL, 0xC266E96FUL,
	0x6FE4AC98UL, 0xB173ECC0UL, 0xBC60B42AUL, 0x953498DAUL,
	0xFBA1AE12UL, 0x2D4BD736UL, 0x0F25FAABUL, 0xA4F3FCEBUL,
	0xE2969123UL, 0x257F0C3DUL, 0x9348AF49UL, 0x361400BCUL,
	0xE8816F4AUL, 0x3814F200UL, 0xA3F94043UL, 0x9C7A54C2UL,
	0xBC704F57UL, 0xDA41E7F9UL, 0xC25AD33AUL, 0x54F4A084UL,
	0xB17F5505UL, 0x59357CBEUL, 0xEDBD15C8UL, 0x7F97C5ABUL,
	0xBA5AC7B5UL, 0xB6F6DEAFUL, 0x3A479C3AUL, 0x5302DA25UL,
	0x653D7E6AUL, 0x54268D49UL, 0x51A477EAUL, 0x5017D55BUL,
	0xD7D25D88UL, 0x44136C76UL, 0x0404A8C8UL, 0xB8E5A121UL,
	0xB81A928AUL, 0x60ED5869UL, 0x97C55B96UL, 0xEAEC991BUL,
	0x29935913UL, 0x01FDB7F1UL, 0x088E8DFAUL, 0x9AB6F6F5UL,
	0x3B4CBF9FUL, 0x4A5DE3ABUL, 0xE6051D35UL, 0xA0E1D855UL,
	0xD36B4CF1UL, 0xF544EDEBUL, 0xB0E93524UL, 0xBEBB8FBDUL,
	0xA2D762CFUL, 0x49C92F54UL, 0x38B5F331UL, 0x7128A454UL,
	0x48392905UL, 0xA65B1DB8UL, 0x851C97BDUL, 0xD675CF2FUL
},

{
	0x85E04019UL, 0x332BF567UL, 0x662DBFFFUL, 0xCFC65693UL,
	0x2A8D7F6FUL, 0xAB9BC912UL, 0xDE6008A1UL, 0x2028DA1FUL,
	0x0227BCE7UL, 0x4D642916UL, 0x18FAC300UL, 0x50F18B82UL,
	0x2CB2CB11UL, 0xB232E75CUL, 0x4B3695F2UL, 0xB28707DEUL,
	0xA05FBCF6UL, 0xCD4181E9UL, 0xE150210CUL, 0xE24EF1BDUL,
	0xB168C381UL, 0xFDE4E789UL, 0x5C79B0D8UL, 0x1E8BFD43UL,
	0x4D495001UL, 0x38BE4341UL, 0x913CEE1DUL, 0x92A79C3FUL,
	0x089766BEUL, 0xBAEEADF4UL, 0x1286BECFUL, 0xB6EACB19UL,
	0x2660C200UL, 0x7565BDE4UL, 0x64241F7AUL, 0x8248DCA9UL,
	0xC3B3AD66UL, 0x28136086UL, 0x0BD8DFA8UL, 0x356D1CF2UL,
	0x107789BEUL, 0xB3B2E9CEUL, 0x0502AA8FUL, 0x0BC0351EUL,
	0x166BF52AUL, 0xEB12FF82UL, 0xE3486911UL, 0xD34D7516UL,
	0x4E7B3AFFUL, 0x5F43671BUL, 0x9CF6E037UL, 0x4981AC83UL,
	0x334266CEUL, 0x8C9341B7UL, 0xD0D854C0UL, 0xCB3A6C88UL,
	0x47BC2829UL, 0x4725BA37UL, 0xA66AD22BUL, 0x7AD61F1EUL,
	0x0C5CBAFAUL, 0x4437F107UL, 0xB6E79962UL, 0x42D2D816UL,
	0x0A961288UL, 0xE1A5C06EUL, 0x13749E67UL, 0x72FC081AUL,
	0xB1D139F7UL, 0xF9583745UL, 0xCF19DF58UL, 0xBEC3F756UL,
	0xC06EBA30UL, 0x07211B24UL, 0x45C28829UL, 0xC95E317FUL,
	0xBC8EC511UL, 0x38BC46E9UL, 0xC6E6FA14UL, 0xBAE8584AUL,
	0xAD4EBC46UL, 0x468F508BUL, 0x7829435FUL, 0xF124183BUL,
	0x821DBA9FUL, 0xAFF60FF4UL, 0xEA2C4E6DUL, 0x16E39264UL,
	0x92544A8BUL, 0x009B4FC3UL, 0xABA68CEDUL, 0x9AC96F78UL,
	0x06A5B79AUL, 0xB2856E6EUL, 0x1AEC3CA9UL, 0xBE838688UL,
	0x0E0804E9UL, 0x55F1BE56UL, 0xE7E5363BUL, 0xB3A1F25DUL,
	0xF7DEBB85UL, 0x61FE033CUL, 0x16746233UL, 0x3C034C28UL,
	0xDA6D0C74UL, 0x79AAC56CUL, 0x3CE4E1ADUL, 0x51F0C802UL,
	0x98F8F35AUL, 0x1626A49FUL, 0xEED82B29UL, 0x1D382FE3UL,
	0x0C4FB99AUL, 0xBB325778UL, 0x3EC6D97BUL, 0x6E77A6A9UL,
	0xCB658B5CUL, 0xD45230C7UL, 0x2BD1408BUL, 0x60C03EB7UL,
	0xB9068D78UL, 0xA33754F4UL, 0xF430C87DUL, 0xC8A71302UL,
	0xB96D8C32UL, 0xEBD4E7BEUL, 0xBE8B9D2DUL, 0x7979FB06UL,
	0xE7225308UL, 0x8B75CF77UL, 0x11EF8DA4UL, 0xE083C858UL,
	0x8D6B786FUL, 0x5A6317A6UL, 0xFA5CF7A0UL, 0x5DDA0033UL,
	0xF28EBFB0UL, 0xF5B9C310UL, 0xA0EAC280UL, 0x08B9767AUL,
	0xA3D9D2B0UL, 0x79D34217UL, 0x021A718DUL, 0x9AC6336AUL,
	0x2711FD60UL, 0x438050E3UL, 0x069908A8UL, 0x3D7FEDC4UL,
	0x826D2BEFUL, 0x4EEB8476UL, 0x488DCF25UL, 0x36C9D566UL,
	0x28E74E41UL, 0xC2610ACAUL, 0x3D49A9CFUL, 0xBAE3B9DFUL,
	0xB65F8DE6UL, 0x92AEAF64UL, 0x3AC7D5E6UL, 0x9EA80509UL,
	0xF22B017DUL, 0xA4173F70UL, 0xDD1E16C3UL, 0x15E0D7F9UL,
	0x50B1B887UL, 0x2B9F4FD5UL, 0x625ABA82UL, 0x6A017962UL,
	0x2EC01B9CUL, 0x15488AA9UL, 0xD716E740UL, 0x40055A2CUL,
	0x93D29A22UL, 0xE32DBF9AUL, 0x058745B9UL, 0x3453DC1EUL,
	0xD699296EUL, 0x496CFF6FUL, 0x1C9F4986UL, 0xDFE2ED07UL,
	0xB87242D1UL, 0x19DE7EAEUL, 0x053E561AUL, 0x15AD6F8CUL,
	0x66626C1CUL, 0x7154C24CUL, 0xEA082B2AUL, 0x93EB2939UL,
	0x17DCB0F0UL, 0x58D4F2AEUL, 0x9EA294FBUL, 0x52CF564CUL,
	0x9883FE66UL, 0x2EC40581UL, 0x763953C3UL, 0x01D6692EUL,
	0xD3A0C108UL, 0xA1E7160EUL, 0xE4F2DFA6UL, 0x693ED285UL,
	0x74904698UL, 0x4C2B0EDDUL, 0x4F757656UL, 0x5D393378UL,
	0xA132234FUL, 0x3D321C5DUL, 0xC3F5E194UL, 0x4B269301UL,
	0xC79F022FUL, 0x3C997E7EUL, 0x5E4F9504UL, 0x3FFAFBBDUL,
	0x76F7AD0EUL, 0x296693F4UL, 0x3D1FCE6FUL, 0xC61E45BEUL,
	0xD3B5AB34UL, 0xF72BF9B7UL, 0x1B0434C0UL, 0x4E72B567UL,
	0x5592A33DUL, 0xB5229301UL, 0xCFD2A87FUL, 0x60AEB767UL,
	0x1814386BUL, 0x30BCC33DUL, 0x38A0C07DUL, 0xFD1606F2UL,
	0xC363519BUL, 0x589DD390UL, 0x5479F8E6UL, 0x1CB8D647UL,
	0x97FD61A9UL, 0xEA7759F4UL, 0x2D57539DUL, 0x569A58CFUL,
	0xE84E63ADUL, 0x462E1B78UL, 0x6580F87EUL, 0xF3817914UL,
	0x91DA55F4UL, 0x40A230F3UL, 0xD1988F35UL, 0xB6E318D2UL,
	0x3FFA50BCUL, 0x3D40F021UL, 0xC3C0BDAEUL, 0x4958C24CUL,
	0x518F36B2UL, 0x84B1D370UL, 0x0FEDCE83UL, 0x878DDADAUL,
	0xF2A279C7UL, 0x94E01BE8UL, 0x90716F4BUL, 0x954B8AA3UL
},

{
	0xE216300DUL, 0xBBDDFFFCUL, 0xA7EBDABDUL, 0x35648095UL,
	0x7789F8B7UL, 0xE6C1121BUL, 0x0E241600UL, 0x052CE8B5UL,
	0x11A9CFB0UL, 0xE5952F11UL, 0xECE7990AUL, 0x9386D174UL,
	0x2A42931CUL, 0x76E38111UL, 0xB12DEF3AUL, 0x37DDDDFCUL,
	0xDE9ADEB1UL, 0x0A0CC32CUL, 0xBE197029UL, 0x84A00940UL,
	0xBB243A0FUL, 0xB4D137CFUL, 0xB44E79F0UL, 0x049EEDFDUL,
	0x0B15A15DUL, 0x480D3168UL, 0x8BBBDE5AUL, 0x669DED42UL,
	0xC7ECE831UL, 0x3F8F95E7UL, 0x72DF191BUL, 0x7580330DUL,
	0x94074251UL, 0x5C7DCDFAUL, 0xABBE6D63UL, 0xAA402164UL,
	0xB301D40AUL, 0x02E7D1CAUL, 0x53571DAEUL, 0x7A3182A2UL,
	0x12A8DDECUL, 0xFDAA335DUL, 0x176F43E8UL, 0x71FB46D4UL,
	0x38129022UL, 0xCE949AD4UL, 0xB84769ADUL, 0x965BD862UL,
	0x82F3D055UL, 0x66FB9767UL, 0x15B80B4EUL, 0x1D5B47A0UL,
	0x4CFDE06FUL, 0xC28EC4B8UL, 0x57E8726EUL, 0x647A78FCUL,
	0x99865D44UL, 0x608BD593UL, 0x6C200E03UL, 0x39DC5FF6UL,
	0x5D0B00A3UL, 0xAE63AFF2UL, 0x7E8BD632UL, 0x70108C0CUL,
	0xBBD35049UL, 0x2998DF04UL, 0x980CF42AUL, 0x9B6DF491UL,
	0x9E7EDD53UL, 0x06918548UL, 0x58CB7E07UL, 0x3B74EF2EUL,
	0x522FFFB1UL, 0xD24708CCUL, 0x1C7E27CDUL, 0xA4EB215BUL,
	0x3CF1D2E2UL, 0x19B47A38UL, 0x424F7618UL, 0x35856039UL,
	0x9D17DEE7UL, 0x27EB35E6UL, 0xC9AFF67BUL, 0x36BAF5B8UL,
	0x09C467CDUL, 0xC18910B1UL, 0xE11DBF7BUL, 0x06CD1AF8UL,
	0x7170C608UL, 0x2D5E3354UL, 0xD4DE495AUL, 0x64C6D006UL,
	0xBCC0C62CUL, 0x3DD00DB3UL, 0x708F8F34UL, 0x77D51B42UL,
	0x264F620FUL, 0x24B8D2BFUL, 0x15C1B79EUL, 0x46A52564UL,
	0xF8D7E54EUL, 0x3E378160UL, 0x7895CDA5UL, 0x859C15A5UL,
	0xE6459788UL, 0xC37BC75FUL, 0xDB07BA0CUL, 0x0676A3ABUL,
	0x7F229B1EUL, 0x31842E7BUL, 0x24259FD7UL, 0xF8BEF472UL,
	0x835FFCB8UL, 0x6DF4C1F2UL, 0x96F5B195UL, 0xFD0AF0FCUL,
	0xB0FE134CUL, 0xE2506D3DUL, 0x4F9B12EAUL, 0xF215F225UL,
	0xA223736FUL, 0x9FB4C428UL, 0x25D04979UL, 0x34C713F8UL,
	0xC4618187UL, 0xEA7A6E98UL, 0x7CD16EFCUL, 0x1436876CUL,
	0xF1544107UL, 0xBEDEEE14UL, 0x56E9AF27UL, 0xA04AA441UL,
	0x3CF7C899UL, 0x92ECBAE6UL, 0xDD67016DUL, 0x151682EBUL,
	0xA842EEDFUL, 0xFDBA60B4UL, 0xF1907B75UL, 0x20E3030FUL,
	0x24D8C29EUL, 0xE139673BUL, 0xEFA63FB8UL, 0x71873054UL,
	0xB6F2CF3BUL, 0x9F326442UL, 0xCB15A4CCUL, 0xB01A4504UL,
	0xF1E47D8DUL, 0x844A1BE5UL, 0xBAE7DFDCUL, 0x42CBDA70UL,
	0xCD7DAE0AUL, 0x57E85B7AUL, 0xD53F5AF6UL, 0x20CF4D8CUL,
	0xCEA4D428UL, 0x79D130A4UL, 0x3486EBFBUL, 0x33D3CDDCUL,
	0x77853B53UL, 0x37EFFCB5UL, 0xC5068778UL, 0xE580B3E6UL,
	0x4E68B8F4UL, 0xC5C8B37EUL, 0x0D809EA2UL, 0x398FEB7CUL,
	0x132A4F94UL, 0x43B7950EUL, 0x2FEE7D1CUL, 0x223613BDUL,
	0xDD06CAA2UL, 0x37DF932BUL, 0xC4248289UL, 0xACF3EBC3UL,
	0x5715F6B7UL, 0xEF3478DDUL, 0xF267616FUL, 0xC148CBE4UL,
	0x9052815EUL, 0x5E410FABUL, 0xB48A2465UL, 0x2EDA7FA4UL,
	0xE87B40E4UL, 0xE98EA084UL, 0x5889E9E1UL, 0xEFD390FCUL,
	0xDD07D35BUL, 0xDB485694UL, 0x38D7E5B2UL, 0x57720101UL,
	0x730EDEBCUL, 0x5B643113UL, 0x94917E4FUL, 0x503C2FBAUL,
	0x646F1282UL, 0x7523D24AUL, 0xE0779695UL, 0xF9C17A8FUL,
	0x7A5B2121UL, 0xD187B896UL, 0x29263A4DUL, 0xBA510CDFUL,
	0x81F47C9FUL, 0xAD1163EDUL, 0xEA7B5965UL, 0x1A00726EUL,
	0x11403092UL, 0x00DA6D77UL, 0x4A0CDD61UL, 0xAD1F4603UL,
	0x605BDFB0UL, 0x9EEDC364UL, 0x22EBE6A8UL, 0xCEE7D28AUL,
	0xA0E736A0UL, 0x5564A6B9UL, 0x10853209UL, 0xC7EB8F37UL,
	0x2DE705CAUL, 0x8951570FUL, 0xDF09822BUL, 0xBD691A6CUL,
	0xAA12E4F2UL, 0x87451C0FUL, 0xE0F6A27AUL, 0x3ADA4819UL,
	0x4CF1764FUL, 0x0D771C2BUL, 0x67CDB156UL, 0x350D8384UL,
	0x5938FA0FUL, 0x42399EF3UL, 0x36997B07UL, 0x0E84093DUL,
	0x4AA93E61UL, 0x8360D87BUL, 0x1FA98B0CUL, 0x1149382CUL,
	0xE97625A5UL, 0x0614D1B7UL, 0x0E25244BUL, 0x0C768347UL,
	0x589E8D82UL, 0x0D2059D1UL, 0xA466BB1EUL, 0xF8DA0A82UL,
	0x04F19130UL, 0xBA6E4EC0UL, 0x99265164UL, 0x1EE7230DUL,
	0x50B2AD80UL, 0xEAEE6801UL, 0x8DB2A283UL, 0xEA8BF59EUL
}};

/* CAST uses three different round functions */
#define f1(l, r, km, kr) \
	t = rotlVariable(km + r, kr); \
	l ^= ((S[0][U8a(t)] ^ S[1][U8b(t)]) - \
	 S[2][U8c(t)]) + S[3][U8d(t)];
#define f2(l, r, km, kr) \
	t = rotlVariable(km ^ r, kr); \
	l ^= ((S[0][U8a(t)] - S[1][U8b(t)]) + \
	 S[2][U8c(t)]) ^ S[3][U8d(t)];
#define f3(l, r, km, kr) \
	t = rotlVariable(km - r, kr); \
	l ^= ((S[0][U8a(t)] + S[1][U8b(t)]) ^ \
	 S[2][U8c(t)]) - S[3][U8d(t)];

#define F1(l, r, i, j) f1(l, r, K[i], K[i+j])
#define F2(l, r, i, j) f2(l, r, K[i], K[i+j])
#define F3(l, r, i, j) f3(l, r, K[i], K[i+j])


void Cast5Encrypt (const byte *inBlock, byte *outBlock, CAST_KEY *key)
{
	word32 l = BE32 (((word32 *)inBlock)[0]);
	word32 r = BE32 (((word32 *)inBlock)[1]);
	word32 *K = key->K;
	word32 t;

	/* Do the work */
	F1(l, r,  0, 16);
	F2(r, l,  1, 16);
	F3(l, r,  2, 16);
	F1(r, l,  3, 16);
	F2(l, r,  4, 16);
	F3(r, l,  5, 16);
	F1(l, r,  6, 16);
	F2(r, l,  7, 16);
	F3(l, r,  8, 16);
	F1(r, l,  9, 16);
	F2(l, r, 10, 16);
	F3(r, l, 11, 16);
	F1(l, r, 12, 16);
	F2(r, l, 13, 16);
	F3(l, r, 14, 16);
	F1(r, l, 15, 16);

	/* Put l,r into outblock */
	((word32 *)outBlock)[0] = BE32 (r);
	((word32 *)outBlock)[1] = BE32 (l);
}


void Cast5Decrypt (const byte *inBlock, byte *outBlock, CAST_KEY *key)
{
	word32 r = BE32 (((word32 *)inBlock)[0]);
	word32 l = BE32 (((word32 *)inBlock)[1]);
	word32 *K = key->K;
	word32 t;

	/* Only do full 16 rounds if key length > 80 bits */
	F1(r, l, 15, 16);
	F3(l, r, 14, 16);
	F2(r, l, 13, 16);
	F1(l, r, 12, 16);
	F3(r, l, 11, 16);
	F2(l, r, 10, 16);
	F1(r, l,  9, 16);
	F3(l, r,  8, 16);
	F2(r, l,  7, 16);
	F1(l, r,  6, 16);
	F3(r, l,  5, 16);
	F2(l, r,  4, 16);
	F1(r, l,  3, 16);
	F3(l, r,  2, 16);
	F2(r, l,  1, 16);
	F1(l, r,  0, 16);
	/* Put l,r into outblock */
	((word32 *)outBlock)[0] = BE32 (l);
	((word32 *)outBlock)[1] = BE32 (r);
	/* Wipe clean */
	t = l = r = 0;
}

void Cast5SetKey (CAST_KEY *key, unsigned int keylength, const byte *userKey)
{
	unsigned int i;
	word32 *K = key->K;
	word32 X[4], Z[4];

	X[0] = BE32 (((word32 *)userKey)[0]);
	X[1] = BE32 (((word32 *)userKey)[1]);
	X[2] = BE32 (((word32 *)userKey)[2]);
	X[3] = BE32 (((word32 *)userKey)[3]);

#define x(i) GETBYTE(X[i/4], 3-i%4)
#define z(i) GETBYTE(Z[i/4], 3-i%4)

	for (i=0; i<=16; i+=16)
	{
		// this part is copied directly from RFC 2144 (with some search and replace) by Wei Dai
		Z[0] = X[0] ^ S[4][x(0xD)] ^ S[5][x(0xF)] ^ S[6][x(0xC)] ^ S[7][x(0xE)] ^ S[6][x(0x8)];
		Z[1] = X[2] ^ S[4][z(0x0)] ^ S[5][z(0x2)] ^ S[6][z(0x1)] ^ S[7][z(0x3)] ^ S[7][x(0xA)];
		Z[2] = X[3] ^ S[4][z(0x7)] ^ S[5][z(0x6)] ^ S[6][z(0x5)] ^ S[7][z(0x4)] ^ S[4][x(0x9)];
		Z[3] = X[1] ^ S[4][z(0xA)] ^ S[5][z(0x9)] ^ S[6][z(0xB)] ^ S[7][z(0x8)] ^ S[5][x(0xB)];
		K[i+0] = S[4][z(0x8)] ^ S[5][z(0x9)] ^ S[6][z(0x7)] ^ S[7][z(0x6)] ^ S[4][z(0x2)];
		K[i+1] = S[4][z(0xA)] ^ S[5][z(0xB)] ^ S[6][z(0x5)] ^ S[7][z(0x4)] ^ S[5][z(0x6)];
		K[i+2] = S[4][z(0xC)] ^ S[5][z(0xD)] ^ S[6][z(0x3)] ^ S[7][z(0x2)] ^ S[6][z(0x9)];
		K[i+3] = S[4][z(0xE)] ^ S[5][z(0xF)] ^ S[6][z(0x1)] ^ S[7][z(0x0)] ^ S[7][z(0xC)];
		X[0] = Z[2] ^ S[4][z(0x5)] ^ S[5][z(0x7)] ^ S[6][z(0x4)] ^ S[7][z(0x6)] ^ S[6][z(0x0)];
		X[1] = Z[0] ^ S[4][x(0x0)] ^ S[5][x(0x2)] ^ S[6][x(0x1)] ^ S[7][x(0x3)] ^ S[7][z(0x2)];
		X[2] = Z[1] ^ S[4][x(0x7)] ^ S[5][x(0x6)] ^ S[6][x(0x5)] ^ S[7][x(0x4)] ^ S[4][z(0x1)];
		X[3] = Z[3] ^ S[4][x(0xA)] ^ S[5][x(0x9)] ^ S[6][x(0xB)] ^ S[7][x(0x8)] ^ S[5][z(0x3)];
		K[i+4] = S[4][x(0x3)] ^ S[5][x(0x2)] ^ S[6][x(0xC)] ^ S[7][x(0xD)] ^ S[4][x(0x8)];
		K[i+5] = S[4][x(0x1)] ^ S[5][x(0x0)] ^ S[6][x(0xE)] ^ S[7][x(0xF)] ^ S[5][x(0xD)];
		K[i+6] = S[4][x(0x7)] ^ S[5][x(0x6)] ^ S[6][x(0x8)] ^ S[7][x(0x9)] ^ S[6][x(0x3)];
		K[i+7] = S[4][x(0x5)] ^ S[5][x(0x4)] ^ S[6][x(0xA)] ^ S[7][x(0xB)] ^ S[7][x(0x7)];
		Z[0] = X[0] ^ S[4][x(0xD)] ^ S[5][x(0xF)] ^ S[6][x(0xC)] ^ S[7][x(0xE)] ^ S[6][x(0x8)];
		Z[1] = X[2] ^ S[4][z(0x0)] ^ S[5][z(0x2)] ^ S[6][z(0x1)] ^ S[7][z(0x3)] ^ S[7][x(0xA)];
		Z[2] = X[3] ^ S[4][z(0x7)] ^ S[5][z(0x6)] ^ S[6][z(0x5)] ^ S[7][z(0x4)] ^ S[4][x(0x9)];
		Z[3] = X[1] ^ S[4][z(0xA)] ^ S[5][z(0x9)] ^ S[6][z(0xB)] ^ S[7][z(0x8)] ^ S[5][x(0xB)];
		K[i+8] = S[4][z(0x3)] ^ S[5][z(0x2)] ^ S[6][z(0xC)] ^ S[7][z(0xD)] ^ S[4][z(0x9)];
		K[i+9] = S[4][z(0x1)] ^ S[5][z(0x0)] ^ S[6][z(0xE)] ^ S[7][z(0xF)] ^ S[5][z(0xC)];
		K[i+10] = S[4][z(0x7)] ^ S[5][z(0x6)] ^ S[6][z(0x8)] ^ S[7][z(0x9)] ^ S[6][z(0x2)];
		K[i+11] = S[4][z(0x5)] ^ S[5][z(0x4)] ^ S[6][z(0xA)] ^ S[7][z(0xB)] ^ S[7][z(0x6)];
		X[0] = Z[2] ^ S[4][z(0x5)] ^ S[5][z(0x7)] ^ S[6][z(0x4)] ^ S[7][z(0x6)] ^ S[6][z(0x0)];
		X[1] = Z[0] ^ S[4][x(0x0)] ^ S[5][x(0x2)] ^ S[6][x(0x1)] ^ S[7][x(0x3)] ^ S[7][z(0x2)];
		X[2] = Z[1] ^ S[4][x(0x7)] ^ S[5][x(0x6)] ^ S[6][x(0x5)] ^ S[7][x(0x4)] ^ S[4][z(0x1)];
		X[3] = Z[3] ^ S[4][x(0xA)] ^ S[5][x(0x9)] ^ S[6][x(0xB)] ^ S[7][x(0x8)] ^ S[5][z(0x3)];
		K[i+12] = S[4][x(0x8)] ^ S[5][x(0x9)] ^ S[6][x(0x7)] ^ S[7][x(0x6)] ^ S[4][x(0x3)];
		K[i+13] = S[4][x(0xA)] ^ S[5][x(0xB)] ^ S[6][x(0x5)] ^ S[7][x(0x4)] ^ S[5][x(0x7)];
		K[i+14] = S[4][x(0xC)] ^ S[5][x(0xD)] ^ S[6][x(0x3)] ^ S[7][x(0x2)] ^ S[6][x(0x8)];
		K[i+15] = S[4][x(0xE)] ^ S[5][x(0xF)] ^ S[6][x(0x1)] ^ S[7][x(0x0)] ^ S[7][x(0xD)];
	}

	for (i=16; i<32; i++)
		K[i] &= 0x1f;
}
