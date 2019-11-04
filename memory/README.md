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

## Metrics with debug build

Meson setup command:

```
$ meson build -Db_sanitize=address
```

Metrics(last 3 columns are percentiles):

```
Metric                                            Average  95th     85th     70th
chunked_pool_slist_step_100.full_deallocation     4614392  4745798  4731786  4625041
chunked_pool_slist_step_100.items_deallocation    4522524  4566743  4550525  4532319
chunked_pool_slist_step_100.primary_allocation    4858742  4984816  4957588  4927504
chunked_pool_slist_step_100.secondary_allocation  4546298  4600255  4575667  4551915
chunked_pool_slist_step_10.full_deallocation      4784704  4916245  4896609  4792468
chunked_pool_slist_step_10.items_deallocation     4519919  4559602  4545876  4530464
chunked_pool_slist_step_10.primary_allocation     5185160  5322521  5283572  5251408
chunked_pool_slist_step_10.secondary_allocation   4541794  4594072  4570445  4548991
chunked_pool_slist_step_5.full_deallocation       4996285  5118424  5099038  5005116
chunked_pool_slist_step_5.items_deallocation      4533674  4584663  4554387  4531822
chunked_pool_slist_step_5.primary_allocation      5505771  5646813  5606338  5558011
chunked_pool_slist_step_5.secondary_allocation    4552257  4601473  4574777  4549250
malloc_slist_nseq.primary_allocation              6008113  6219742  6120127  6061496
malloc_slist_nseq.primary_deallocation            5638870  5781845  5758701  5672141
malloc_slist_nseq.secondary_allocation            6017973  6155687  6122107  6002579
malloc_slist_nseq.secondary_deallocation          5599621  5733783  5705157  5597515
malloc_slist_seq.primary_allocation               6081259  6235604  6195114  6149818
malloc_slist_seq.primary_deallocation             5496243  5625316  5607317  5508311
malloc_slist_seq.secondary_allocation             6019273  6155375  6129839  6006378
malloc_slist_seq.secondary_deallocation           5457531  5590748  5564400  5447767
pool_slist.full_deallocation                      5584354  5721303  5698534  5590640
pool_slist.items_deallocation                     4542793  4631668  4597687  4546879
pool_slist.primary_allocation                     6238546  6465836  6412681  6288175
pool_slist.secondary_allocation                   4545746  4615961  4579468  4555027
```

## Metrics with release build

Meson setup command:

```
$ meson build --buildtype release
```

Metrics(last 3 columns are percentiles):

```
Metric                                            Average  95th     85th     70th
chunked_pool_slist_step_100.full_deallocation     4293284  4336674  4327828  4308170
chunked_pool_slist_step_100.items_deallocation    4206988  4242830  4236429  4226245
chunked_pool_slist_step_100.primary_allocation    4333488  4386295  4378258  4357905
chunked_pool_slist_step_100.secondary_allocation  4206612  4241783  4236299  4226480
chunked_pool_slist_step_10.full_deallocation      4257269  4279733  4274022  4264931
chunked_pool_slist_step_10.items_deallocation     4207528  4227622  4222807  4213923
chunked_pool_slist_step_10.primary_allocation     4429482  4468629  4460309  4440729
chunked_pool_slist_step_10.secondary_allocation   4209275  4230136  4223917  4215779
chunked_pool_slist_step_5.full_deallocation       4338126  4384021  4372069  4352546
chunked_pool_slist_step_5.items_deallocation      4198252  4232379  4227205  4217789
chunked_pool_slist_step_5.primary_allocation      4521826  4575952  4566530  4545512
chunked_pool_slist_step_5.secondary_allocation    4202432  4238952  4233595  4222957
malloc_slist_nseq.primary_allocation              4813114  4894142  4880552  4847599
malloc_slist_nseq.primary_deallocation            4325638  4368451  4356696  4336058
malloc_slist_nseq.secondary_allocation            4448787  4481797  4475225  4465002
malloc_slist_nseq.secondary_deallocation          4327162  4356236  4348686  4338216
malloc_slist_seq.primary_allocation               4686912  4750573  4721619  4685519
malloc_slist_seq.primary_deallocation             4343031  4365193  4359643  4350270
malloc_slist_seq.secondary_allocation             4431661  4453304  4445560  4435509
malloc_slist_seq.secondary_deallocation           4338610  4367343  4360653  4348891
pool_slist.full_deallocation                      4329193  4365207  4360251  4349620
pool_slist.items_deallocation                     4231954  4266386  4259469  4247293
pool_slist.primary_allocation                     4676560  4753465  4723289  4686265
pool_slist.secondary_allocation                   4208264  4244242  4238910  4228791
```
