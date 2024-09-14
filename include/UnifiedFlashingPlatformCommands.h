/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2022, halal-beef <umer.uddin@mentallysanemainliners.org>
 */

#ifndef UNIFIEDFLASHINGPLATFORMCOMMANDS_H_ /* Include guard */
#define UNIFIEDFLASHINGPLATFORMCOMMANDS_H_

/*
 * Not valid commands
 */
#define Signature "NOK" // NOK
#define ExtendedMessageSignature Signature "X" // NOKX
#define CommonExtendedMessageSignature ExtendedMessageSignature "C" // NOKXC
#define UFPExtendedMessageSignature ExtendedMessageSignature "F" // NOKXF

/*
 * Normal commands
 */
#define FlashSignature Signature "F" // NOKF
#define HelloSignature Signature "I" // NOKI
#define MassStorageSignature Signature "M" // NOKM
#define TelemetryEndSignature Signature "N" // NOKN
#define RebootSignature Signature "R" // NOKR
#define TelemetryStartSignature Signature "S" // NOKS
#define GetGPTSignature Signature "T" // NOKT
#define InfoQuerySignature Signature "V" // NOKV
#define ShutdownSignature Signature "Z" // NOKZ

/*
 * Common extended commands
 */
#define SwitchModeSignature CommonExtendedMessageSignature "B" // NOKXCB
#define ClearScreenSignature CommonExtendedMessageSignature "C" // NOKXCC
#define GetDirectoryEntriesSignature CommonExtendedMessageSignature "D" // NOKXCD
#define EchoSignature CommonExtendedMessageSignature "E" // NOKXCE
#define GetFileSignature CommonExtendedMessageSignature "F" // NOKXCF
#define DisplayCustomMessageSignature CommonExtendedMessageSignature "M" // NOKXM
#define PutFileSignature CommonExtendedMessageSignature "P" // NOKXP
#define BenchmarkTestsSignature CommonExtendedMessageSignature "T" // NOKXT

/*
 * UFP extended commands
 */
#define AsyncFlashModeSignature UFPExtendedMessageSignature "F" // NOKXFF
#define UnlockSignature UFPExtendedMessageSignature "I" // NOKXFI
#define RelockSignature UFPExtendedMessageSignature "O" // NOKXFO
#define ReadParamSignature UFPExtendedMessageSignature "R" // NOKXFR
#define SecureFlashSignature UFPExtendedMessageSignature "S" // NOKXFS
#define TelemetryReadSignature UFPExtendedMessageSignature "T" // NOKXFT
#define WriteParamSignature UFPExtendedMessageSignature "W" // NOKXFW
#define GetLogsSignature UFPExtendedMessageSignature "X" // NOKXFX

#endif // UNIFIEDFLASHINGPLATFORMCOMMANDS_H_