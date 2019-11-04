# Memory management strategies performance

## Hardware details

Collected with `lshw`:

```
H/W path           Device      Class          Description
=========================================================
                               system         K53SK
/0                             bus            K53SK
/0/0                           memory         64KiB BIOS
/0/4                           processor      Intel(R) Core(TM) i7-2670QM CPU @ 2.20GHz
/0/4/5                         memory         32KiB L1 cache
/0/4/6                         memory         256KiB L2 cache
/0/40                          memory         6GiB System Memory
/0/40/0                        memory         4GiB SODIMM DDR3 Synchronous 1333 MHz (0.8 ns)
/0/40/2                        memory         2GiB SODIMM DDR3 Synchronous 1333 MHz (0.8 ns)
```

## Metrics with release build

Meson setup command:

```
$ meson build --buildtype release
```

Metrics(last 3 columns are percentiles):

```
Metric                                             Average  95th     85th     70th
chunked_pool_slist_step_1000.full_deallocation     4319045  4423492  4390697  4361130
chunked_pool_slist_step_1000.items_deallocation    4229462  4319245  4295127  4277429
chunked_pool_slist_step_1000.primary_allocation    4354763  4468555  4429046  4398771
chunked_pool_slist_step_1000.secondary_allocation  4229667  4324497  4296602  4274837
chunked_pool_slist_step_100.full_deallocation      4321830  4426767  4393546  4365183
chunked_pool_slist_step_100.items_deallocation     4230661  4319536  4296019  4278564
chunked_pool_slist_step_100.primary_allocation     4360985  4472451  4437614  4407467
chunked_pool_slist_step_100.secondary_allocation   4230088  4324689  4297377  4275686
chunked_pool_slist_step_10.full_deallocation       4275782  4361379  4337618  4319960
chunked_pool_slist_step_10.items_deallocation      4219722  4306542  4283580  4265148
chunked_pool_slist_step_10.primary_allocation      4449531  4558430  4521122  4489954
chunked_pool_slist_step_10.secondary_allocation    4220173  4307406  4286008  4263830
chunked_pool_slist_step_5.full_deallocation        4365886  4471861  4437945  4407966
chunked_pool_slist_step_5.items_deallocation       4222652  4313370  4288916  4270646
chunked_pool_slist_step_5.primary_allocation       4548694  4656066  4623438  4591133
chunked_pool_slist_step_5.secondary_allocation     4227104  4321672  4295027  4274701
malloc_slist_nseq.primary_allocation               4838652  4989847  4933491  4876707
malloc_slist_nseq.primary_deallocation             4356347  4461735  4423636  4397179
malloc_slist_nseq.secondary_allocation             4476160  4569511  4542961  4522265
malloc_slist_nseq.secondary_deallocation           4355527  4454032  4425430  4398838
malloc_slist_seq.primary_allocation                4711693  4864853  4789564  4745423
malloc_slist_seq.primary_deallocation              4353876  4450218  4425783  4406012
malloc_slist_seq.secondary_allocation              4447598  4544479  4511969  4492099
malloc_slist_seq.secondary_deallocation            4355708  4459179  4422861  4403600
pool_slist.full_deallocation                       4353464  4446110  4420998  4400842
pool_slist.items_deallocation                      4255102  4370975  4316801  4295419
pool_slist.primary_allocation                      4712423  4863472  4789488  4746663
pool_slist.secondary_allocation                    4229291  4322645  4297948  4275830
```
