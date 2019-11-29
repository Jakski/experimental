################################################################################
# Generate statistics
################################################################################

BEGIN {
  # This need to be updated in metrics script as well
  REPEAT = 1000000
  PERCENTILE_SIZE_95 = int(REPEAT * 0.95);
  PERCENTILE_SIZE_85 = int(REPEAT * 0.85);
  PERCENTILE_SIZE_70 = int(REPEAT * 0.70);
  WORKDIR = ENVIRON["MESON_BUILD_ROOT"];
  system("rm -f \"" WORKDIR "/metrics.txt\"");
  system(STARTCMD " \"" WORKDIR "/main\" >> \"" WORKDIR "/metrics.txt\"");
  system("sort -n \"" WORKDIR "/metrics.txt\" > \"" WORKDIR "/metrics_sorted.txt\"");
  system("mv \"" WORKDIR "/metrics_sorted.txt\" \"" WORKDIR "/metrics.txt\"");
}

{
  count[$1]++;
  sum[$1] += $2;
  if (count[$1] == PERCENTILE_SIZE_95)
  {
    percentile_95[$1] = $2;
  }
  if (count[$1] == PERCENTILE_SIZE_85)
  {
    percentile_85[$1] = $2;
  }
  if (count[$1] == PERCENTILE_SIZE_70)
  {
    percentile_70[$1] = $2;
  }
}

END {
  # Prefix with 0 to ease sorting on command line
  print "0Metric Average 95th 85th 70th";
  for (metric in count) {
    printf "%s %d %d %d %d\n", metric, int(sum[metric] / REPEAT), int(percentile_95[metric]), int(percentile_85[metric]), int(percentile_70[metric]);
  }
}
